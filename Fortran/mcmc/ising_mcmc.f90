!! MCMC for the 2d Ising model
!! see <http://rajeshrinet.github.io/blog/2014/ising-model>

#define C_F64  real(kind=c_double)
#define C_I32  integer(kind=c_int)

module ising_model
  use, intrinsic :: iso_c_binding, only: c_double, c_int
  implicit none
  private

  public :: IsingMCMC, Parameters, Observables

  ! double precision kind parameter
  integer, parameter :: f64 = selected_real_kind(c_double)
  integer, parameter :: i32 = selected_int_kind(c_int)

  type, bind(C) :: Parameters
     C_I32 :: thermalization  ! nr of warm-up steps
     C_I32 :: steps           ! nr of MC steps
     C_I32 :: j               ! J / |J| = +-1, the sign of the spin coupling, J
     ! +: ferromagnetic coupling, -: antiferromagnetic coupling
     C_F64 :: t               ! T / |J|, temperature in units of J
     C_F64 :: beta            ! 1 / (T/|J|), inverse temperature in units of J
     C_F64 :: h               ! h / |J|, external magnetic field in units of J
     C_I32 :: L               ! linear dimension of the lattice
  end type Parameters

  ! final physical observables
  type, bind(C) :: Observables
     C_F64 :: energy         ! energy per volume
     C_F64 :: magnetization  ! magnetization per volume
     C_F64 :: specific_heat  ! specific heat per volume
     C_F64 :: susceptibility ! magnetic susceptibility per volume
     C_F64 :: mag_localization ! magnetic localization
  end type Observables

  ! Monte-Carlo intermediate observables
  type :: AuxObservables
     C_F64, dimension(:), pointer :: energy         ! total energy
     C_F64, dimension(:), pointer :: magnetization  ! total magnetization
     C_F64, dimension(:, :), pointer :: mag_localization  ! magnetic localization
  end type AuxObservables

contains

  subroutine initLattice(prm, lattice)
    ! initializes the lattice with random spins
    type(Parameters), intent(in) :: prm
    C_I32, intent(out) :: lattice(prm%L, prm%L)

    C_I32 :: i, j
    C_F64 :: r

    ! initialize each site with random spin (+-1)
    do j = 1, prm%L
       do i = 1, prm%L
          call random_number(r)
          lattice(i,j) = merge(1, -1, r > 0.5)
       end do
    end do
  end subroutine initLattice

  pure function withUpperLim(i, L)
    C_I32, intent(in) :: i, L
    C_I32 :: withUpperLim

    if (i > L) then
       withUpperLim = 1
    else
       withUpperLim = i
    end if
  end function withUpperLim

  pure function withLowerLim(i, L)
    C_I32, intent(in) :: i, L
    C_I32 :: withLowerLim

    if (i < 1) then
       withLowerLim = L
    else
       withLowerLim = i
    end if
  end function withLowerLim

  subroutine flipSite(lattice, prm, i, j)
    ! performs a single Monte Carlo move using Metropolis algorithm

    type(Parameters), intent(in) :: prm
    C_I32, dimension(prm%L,prm%L), intent(inout) :: lattice
    C_I32, intent(in) :: i, j

    logical :: accepted
    C_I32 :: s_O, s_LE, s_RT, s_UP, s_DN, nn_sum
    C_F64 :: delta_E, r0

    ! calculate sum of nearest neighbors with periodic boundaries
    s_O  = lattice(i, j)
    s_LE = lattice(withLowerLim(i-1, prm%L), j)
    s_RT = lattice(withUpperLim(i+1, prm%L), j)
    s_UP = lattice(i, withLowerLim(j-1, prm%L))
    s_DN = lattice(i, withUpperLim(j+1, prm%L))

    nn_sum = s_LE + s_RT + s_UP + s_DN

    ! energy change after flipping the site
    ! s_O_new = -s_O => s_O_new - s_O = -2 * s_O
    delta_E = 2.0D0 * s_O * (prm%j * nn_sum + prm%h)

    ! Metropolis acceptance criterion
    if (delta_E <= 0.0D0) then
       accepted = .true.
    else
       call random_number(r0)
       if (r0 < exp(-prm%beta * delta_E)) then
          accepted = .true.
       else
          accepted = .false.
       end if
    end if

    if (accepted) then
       lattice(i,j) = -lattice(i,j)  ! flip the spin
    end if

  end subroutine flipSite

  subroutine mcmcStep(prm, lattice)
    ! performs a single Monte Carlo sweep over all lattice sites

    type(Parameters), intent(in) :: prm
    C_I32, dimension(prm%L, prm%L), intent(inout) :: lattice

    C_I32 :: i, j

    ! sweep through all lattice sites
    do j = 1, prm%L  ! columns
       do i = 1, prm%L  ! rows
          call flipSite(lattice, prm, i, j)
       end do
    end do
  end subroutine mcmcStep

  pure function calcEnergy(prm, lattice)
    ! calculates the total energy
    type(Parameters), intent(in) :: prm
    C_I32, dimension(prm%L, prm%L), intent(in) :: lattice
    C_F64 :: calcEnergy

    C_I32 :: i, j, s_O, s_RT, s_DN
    C_F64 :: dE_ij

    calcEnergy = 0.0D0

    ! sweep through all lattice sites
    do j = 1, prm%L  ! columns
       do i = 1, prm%L  ! rows
              s_O  = lattice(i, j)
              s_RT = lattice(withUpperLim(i+1, prm%L), j)
              s_DN = lattice(i, withUpperLim(j+1, prm%L))
              dE_ij = - s_O * (prm%j * (s_RT + s_DN) + prm%h)
              calcEnergy = calcEnergy + dE_ij
       end do
    end do
  end function calcEnergy

  pure function calcMagnetization(prm, lattice)
    ! calculates the total magnetization
    type(Parameters), intent(in) :: prm
    C_I32, dimension(prm%L, prm%L), intent(in) :: lattice
    C_F64 :: calcMagnetization

    calcMagnetization = sum(real(lattice, c_double))
  end function calcMagnetization

  subroutine calcMagLocalization(lattice, mag_localization)
    ! calculates the total energy
    C_I32, dimension(:, :), intent(in) :: lattice
    C_F64, dimension(2), intent(out) :: mag_localization

    C_F64 :: m_up_2, m_dn_2, f1_m2_sum
    integer :: i, j

    m_up_2 = 0.0D0
    m_dn_2 = 0.0D0

    ! sweep through all lattice sites
    do j = 1, size(lattice, dim=2)  ! columns
       do i = 1, size(lattice, dim=1) ! rows
          ! m_z = lattice(i, j)
          ! NOTE: |m_z|^2 = 1

          ! m_z = up
          if (lattice(i, j) > 0) then
             m_up_2 = m_up_2 + 1
          else
             ! m_z = dn
             m_dn_2 = m_dn_2 + 1
          end if
       end do
    end do

    f1_m2_sum = 1D0 / (m_up_2 + m_dn_2)
    mag_localization(1) = m_up_2 * f1_m2_sum  ! = P_up
    mag_localization(2) = m_dn_2 * f1_m2_sum  ! = P_dn

  end subroutine calcMagLocalization

  subroutine calcObservables(prm, auxobs, obs)
    ! calculates physical observables

    type(Parameters), intent(in) :: prm
    type(AuxObservables), intent(in) :: auxobs
    type(Observables), intent(out) :: obs

    integer :: vol
    real(kind=f64) :: E_avg, E2_avg, &
         M_avg, M2_avg, &
         P_up, P_dn, &
         f1_steps, f1_V

    vol = prm%L * prm%L
    f1_steps = 1D0 / real(prm%steps, kind=f64)
    f1_V = 1.0D0 / real(vol, kind=f64)

    ! calculate Monte-Carlo averages
    E_avg = sum(auxobs%energy) * f1_steps
    E2_avg = sum(auxobs%energy ** 2) * f1_steps
    ! E4_avg = sum(auxobs%energy ** 4) * f1_steps

    M_avg = sum(auxobs%magnetization) * f1_steps
    M2_avg = sum(auxobs%magnetization ** 2) * f1_steps
    ! M4_avg = sum(auxobs%magnetization ** 4) * f1_steps

    ! store normalized observables
    obs%energy = E_avg * f1_V
    obs%magnetization = M_avg * f1_V
    obs%specific_heat = (E2_avg - E_avg * E_avg) * prm%beta * prm%beta * f1_V
    obs%susceptibility = (M2_avg - M_avg * M_avg) * prm%beta * f1_V

    P_up = sum(auxobs%mag_localization(:, 1)) * f1_steps
    P_dn = sum(auxobs%mag_localization(:, 2)) * f1_steps
    ! magnetic localization entropy
    obs%mag_localization = - (P_up * log(P_up + 1D-10) + P_dn * log(P_dn + 1D-10))
  end subroutine calcObservables

  subroutine IsingMCMC(prm, lattice, energies, magnetizations, maglocalizations, obs) &
    bind(C, name="IsingMCMC")
    ! runs MCMC and calculates the final observables

    type(Parameters), intent(inout) :: prm
    C_I32, dimension(prm%L, prm%L), intent(inout) :: lattice
    C_F64, dimension(prm%steps), target, intent(out) :: energies, magnetizations
    C_F64, dimension(prm%steps, 2), target, intent(out) :: maglocalizations
    type(Observables), intent(out) :: obs

    type(AuxObservables) :: auxobs
    C_F64 :: delta_E, total_E
    C_I32 :: i_step

    auxobs%energy => energies
    auxobs%magnetization => magnetizations
    auxobs%mag_localization => maglocalizations

    prm%beta = 1D0 / prm%t

    delta_E = 0.0D0
    total_E = 0.0D0

    ! initialize the lattice
    call initLattice(prm, lattice)

    ! MCMC warm-up
    do i_step = 1, prm%thermalization
       call mcmcStep(prm, lattice)
    end do

    ! total_E = calcEnergy(prm, lattice)

    ! MCMC main loop
    do i_step = 1, prm%steps
       call mcmcStep(prm, lattice)

       !! calculate aux. observables
       ! energy
       auxobs%energy(i_step) = calcEnergy(prm, lattice)
       ! total_E = total_E + delta_E  ! update total energy
       ! auxobs%energy(i_step) = total_E

       !! magnetization
       auxobs%magnetization(i_step) = calcMagnetization(prm, lattice)

       !! magnetic localization
       call calcMagLocalization(lattice, auxobs%mag_localization(i_step, :))
    end do

    ! calculate final observables
    call calcObservables(prm, auxobs, obs)

  end subroutine IsingMCMC

end module ising_model

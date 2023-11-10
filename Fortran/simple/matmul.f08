!! source <https://stackoverflow.com/a/65367769>

program ConsoleMatMul
  implicit none
  integer :: n,i,j
  integer :: tic,toc
  real(8), allocatable :: a(:,:),b(:,:),c(:,:),xe(:,:)

  n = 2000
  allocate(a(n,n), b(n,n), c(n,n), xe(n,n))
  do i=1,n
     do j =1,n
        a(j,i) = i * 1.0
        b(j,i) = i * 1.0
     enddo
  enddo

  call system_clock(tic)
  call nativemul(a,b,c)
  call system_clock(toc)
  print*,'Fortran with loop (s):', (toc - tic) / 1000.0

  call system_clock(tic)
  c = matmul(a,b)
  call system_clock(toc)
  print*,'Fortran with matmul (s):', (toc - tic) / 1000.0

  ! c = b
  ! xe = 0d0
  ! call system_clock(tic)
  ! call gemm(a,c,xe) ! BLAS MATRIX/MATRIX MUL
  ! call system_clock(toc)
  ! print*,'Fortran with blas3:', (toc - tic) / 1000.0

  deallocate(a,b,c)

contains

  pure subroutine nativemul0(a,b,c)
    real(8), intent(in), allocatable :: a(:,:), b(:,:)
    real(8), intent(out), allocatable :: c(:,:)
    real(8) :: s
    integer :: n, i,j,k
    n = size(a,1)
    if (.not. allocated(c)) allocate(c(n,n))
    do j=1,n
       do i=1,n
          s = 0.0d0
          do k=1,n
             s = s + a(i,k) * b(k,j)
          end do
          c(i,j) = s
       end do
    end do
  end subroutine nativemul0

  pure subroutine nativemul(a,b,c)
    real(8), intent(in) :: a(:,:), b(:,:)
    real(8), intent(out) :: c(:,:)
    integer :: n, i,j
    n = size(a,1)
    do j=1,n
       do i=1,n
          c(i,j) = dot_product(a(i,:),b(:,j))
          ! or  = sum(a(i,:)*b(:,j))
       end do
    end do
  end subroutine nativemul

end program ConsoleMatMul

! gfortran -Wall -Wextra -std=f2018 -pedantic matmul.f08 -o matmul.out && ./matmul.out

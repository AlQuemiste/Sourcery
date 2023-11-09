module mean_module
use, intrinsic :: iso_c_binding, only: c_double, c_int

implicit none

contains

subroutine calculate_mean(a, b, n, m) bind(c)
  integer(c_int), value, intent(in) :: n
  integer(c_int), intent(out) :: m
  real(c_double), intent(in), dimension(n) :: a
  real(c_double), intent(out), dimension(n) :: b
  real(c_double) :: sum_a, mean_a
  integer(c_int) :: i

  m = 42
  sum_a = 0

  do i = 1, n
     sum_a = sum_a + a(i)
  end do

  mean_a = sum_a / real(n)

  do i = 1, n
     b(i) = mean_a
  end do

  print *, "> FORTRAN calculate_mean done."

end subroutine calculate_mean

end module mean_module

! gfortran -shared -fPIC -o libarr_mean.so arr_mean.f08

module fmesh_wrapper

use iso_c_binding, only: c_double, c_int

implicit none

contains

subroutine c_mesh_exp(r_min, r_max, a, N) bind(c)
real(c_double), intent(in) :: r_min
real(c_double), intent(in) :: r_max
real(c_double), intent(in) :: a
integer(c_int), intent(inout) :: N

print *, "subroutine c_mesh_exp(", r_min, ", ", r_max
N = N + 2

end subroutine


end module

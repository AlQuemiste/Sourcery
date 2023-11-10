program pointer_alias
implicit none

real, target :: a(10)
real, pointer :: b(:) => null(), c(:) => null()

a = [1,2,3,4,5,6,7,8,9,10]

b(1:5) => a(1:5)
c(1:5) => a(5:9) 

! b and c both alias element a(5), potential flow dependency
call add_in_place(b,c)
print *, b

a = [1,2,3,4,5,6,7,8,9,10]
b(1:5) => a(1:5)
c(1:5) => a(6:10) 
call add_in_place_v2(b,c)
print *, b

contains

! unsafe if executed in parallel due to write/read order of array b
subroutine add_in_place(b,c)
real, intent(inout) :: b(:)
real, intent(in) :: c(size(b))
!$omp parallel workshare
b = b + c  
!$omp end parallel workshare
end subroutine

! "restrict" version, use of do concurrent implies no data dependencies
! between elements of b and c
subroutine add_in_place_v2(b,c)
real, intent(inout) :: b(:)
real, intent(in) :: c(size(b))
integer :: i
do concurrent (i=1:size(b))
b(i) = b(i) + c(i)
end do
end subroutine

end program

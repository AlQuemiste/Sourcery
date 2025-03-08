module MyStructModule
  use, intrinsic :: iso_c_binding
  implicit none

  ! C-interoperable MyStruct type
  type, bind(C) :: MyStruct
    integer(c_int) :: i
    real(c_double) :: x
    type(c_ptr) :: int_array
    type(c_ptr) :: dbl_array
  end type MyStruct

  ! C-interoperable MyVoid type
  type, bind(c) :: MyVoid
    type(c_ptr) :: my_struct
  end type MyVoid

!   ! declare the interoperable subroutine
!   interface
!     subroutine f_void(my_void) bind(C, name="f_void")
!       import :: MyVoid
!       implicit none
!       type(MyVoid), intent(in) :: my_void
!     end subroutine f_void
!   end interface

contains

  subroutine f_void(my_void, n, d1, d2)  bind(C, name="f_void")
    type(MyVoid), intent(in) :: my_void
    integer(c_int), value, intent(in) :: n, d1, d2

    type(MyStruct), pointer :: my_struct_ptr
    integer(c_int), pointer :: int_array(:)
    real(c_double), pointer :: dbl_array(:, :)

    ! check if the void pointer is associated
    if (.not. c_associated(my_void%my_struct)) then
      print *, "FORTRAN: error: my_struct pointer is null."
      return
    end if

    ! convert the void pointer to a Fortran pointer of type MyStruct
    call c_f_pointer(my_void%my_struct, my_struct_ptr)

    ! use my_struct_ptr as a standard Fortran type(MyStruct)
    print *, "FORTRAN: MyStruct scalars:"
    print *, "  i = ", my_struct_ptr%i
    print *, "  x = ", my_struct_ptr%x

    print *, "FORTRAN: MyStruct arrays:"
    ! arrays: convert C pointers to Fortran pointers
    call c_f_pointer(my_struct_ptr%int_array, int_array, [n])
    call c_f_pointer(my_struct_ptr%dbl_array, dbl_array, [d1, d2])

    ! use arrays
    print *, "  integer array (1D):"
    print *, int_array

    print *, "  double array (2D):"
    print *, dbl_array
  end subroutine f_void
  
  ! create a new MyStruct instance and return a void* pointer
  subroutine create_my_struct(void_ptr) bind(c, name="create_my_struct")
    type(c_ptr), intent(out) :: void_ptr
    type(MyStruct), pointer :: my_struct_ptr

    ! allocate a new instance of MyStruct
    print *, "FORTRAN: allocate a MyStruct instance"
    allocate(my_struct_ptr)

    ! initialize the fields (optional)
    my_struct_ptr%i = 45
    my_struct_ptr%x = 1.147_c_double
    my_struct_ptr%int_array = c_null_ptr
    my_struct_ptr%dbl_array = c_null_ptr

    ! return the C-compatible void* pointer to the allocated instance
    void_ptr = c_loc(my_struct_ptr)
  end subroutine create_my_struct

  ! free the allocated MyStruct instance
  subroutine free_my_struct(void_ptr) bind(c, name="free_my_struct")
    type(c_ptr), intent(in) :: void_ptr
    type(MyStruct), pointer :: my_struct_ptr

    print *, "FORTRAN: deallocate a MyStruct instance"
    ! convert the void* pointer back to a Fortran pointer
    call c_f_pointer(void_ptr, my_struct_ptr)

    ! deallocate the MyStruct instance
    deallocate(my_struct_ptr)
  end subroutine free_my_struct

end module MyStructModule

program ieee

    use, intrinsic :: iso_fortran_env, only: f64=>real64, i64=>int64
    use, intrinsic :: ieee_arithmetic
    implicit none

    real(f64)   :: r, one, arr(10)
    integer(i64) :: i, j, k
    integer(i64), parameter :: n0 = 2
    real(f64) :: inf

    inf = ieee_value(real(f64), ieee_positive_inf)

    one = 1.0_f64
    r = ieee_value(one, ieee_positive_inf)
    print *, r
    i = transfer(r, i)
    print '(b64.64)', i

    arr(1) = inf
    print *, "arr(1) = ", arr(1), "| ", ieee_is_finite(arr(1))

    print *, one / r
    i = transfer(one/r, i)
    print '(b64.64)', i
    print *

    r = ieee_value(one, ieee_negative_inf)
    print *, r
    i = transfer(r, i)
    print '(b64.64)', i

    print *, one / r
    i = transfer(one/r, i)
    print '(b64.64)', i
    print *

    j = 1
    k = foo(j, [1, 2, 3])
    print '(a,i1)', 'result = ', k

  contains

    function foo(n, a)
      integer(i64) :: foo
      integer(i64), intent(in) :: n
      integer, intent(in) :: a(3)
      integer(i64), parameter :: c0 = 3

      foo = n + n0 + c0 + a(1)

    end function foo

end program ieee

! gfortran -Wall -Wextra -std=f2018 -pedantic ieee_arithmetic.f08 -o ieee.out && ./ieee.out

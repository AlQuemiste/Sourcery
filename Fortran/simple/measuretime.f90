program ElapsedTime
  implicit none

  integer(kind=8) :: count_rate, count_max, start, finish
  double precision :: elapsed_time_s

  integer, parameter :: n_times = 1000000
  integer :: i

  ! initialisation of system-clock variables
  call system_clock(start, count_rate, count_max)

  call system_clock(start)
  ! code piece to be measured
  do i = 1, n_times
     if(mod(i, n_times / 100000) == 0) then
        print *, "i: ", i
     endif
  end do
  ! end of code piece
  call system_clock(finish)

  elapsed_time_s = (finish - start) / dble(count_rate)

  write(*, '(A, F0.3, A, I0, A)') "* elapsed time: ", elapsed_time_s, " s = ", int(elapsed_time_s * 1e6), " us"

end program ElapsedTime

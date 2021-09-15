program output
use mpi
implicit none

integer :: err, i, myid, file, intsize
integer :: status(mpi_status_size)
integer, parameter :: count=100
integer, dimension(count) :: buf
integer(kind=mpi_offset_kind) :: disp

call mpi_init(err)
call mpi_comm_rank(mpi_comm_world, myid, err)

do i = 1, count
    buf(i) = myid * count + i
end do

call mpi_file_open(mpi_comm_world, 'test', &
                   mpi_mode_create + mpi_mode_wronly, &
                   mpi_info_null, file, err)

intsize = sizeof(count)
disp = myid * count * intsize

call mpi_file_write_at(file, disp, buf, count, mpi_integer, status, err)

call mpi_file_close(file, err)
call mpi_finalize(err)

end program output

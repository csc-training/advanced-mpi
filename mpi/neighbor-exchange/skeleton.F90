program neighborhood
  use mpi_f08
  implicit none

  integer :: ntasks, cart_id, i, rc
  integer, parameter :: dsize = 4
  integer :: data(0:dsize+1)
  integer :: sendbuf(2), recvbuf(2)
  
  type(mpi_comm) :: cart_comm
  integer, parameter :: ndims = 1
  integer :: dims(1)
  logical :: reorder, periods(1)

  call mpi_init(rc)
  call mpi_comm_size(MPI_COMM_WORLD, ntasks, rc)

  ! TODO create cartesian communicator

  ! TODO end

  call mpi_comm_rank(cart_comm, cart_id, rc)

  ! Initialize data
  data = 0
  do i=1, ntasks
     data(i) = cart_id * dsize + (i - 1)
  end do

  ! print out initial data
  if (cart_id == 0) then
     write(*,*) 'Initial data'
  end if

  call print_buffers(data, cart_comm)

  ! TODO implement halo exchange with neighborhood communication
  ! Copy data to send buffer

  ! Communicate with neighbor

  ! Copy receive buffer to "halo"

  ! TODO end

  ! print out results
  if (cart_id == 0) then
     write(*,*) 'Final data'
  end if

  call print_buffers(data, cart_comm)

  call mpi_finalize(rc)

contains

  subroutine print_buffers(buffer, comm)
    implicit none
    integer, dimension(:), intent(in) :: buffer
    type(mpi_comm), intent(in) :: comm
    integer, dimension(:), allocatable :: printbuffer
    integer :: bufsize, rank, ntasks
    integer :: i

    bufsize = size(buffer)
    call mpi_comm_size(comm, ntasks)
    call mpi_comm_rank(comm, rank)
    allocate(printbuffer(ntasks * bufsize))
    
    call mpi_gather(buffer, bufsize, MPI_INTEGER, &
         & printbuffer, bufsize, MPI_INTEGER, &
         & 0, comm, rc)

    if (rank == 0) then
       do i = 1, ntasks
          write(*,'(A,I3,A,*(I3))') 'Task', i - 1, ': ', printbuffer((i-1)*bufsize+1:i*bufsize)
       end do
       print *
    end if

    deallocate(printbuffer)

  end subroutine print_buffers


end program

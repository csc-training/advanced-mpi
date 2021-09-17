program alltoallw

  use mpi_f08
  implicit none

  integer :: rank, ntasks, rc
  integer :: array(8,8)
  integer :: buf(8,8)
  type(mpi_datatype) :: columntype, rowtype, types(4)
  type(mpi_comm) :: cart_comm
  type(mpi_request) :: reqs(8)
  integer :: src, dest

  integer :: ndims=2
  integer :: dims(2) = [3, 3]
  logical :: periods(2) = [.false., .true.]
  integer :: counts(4) = 1
  integer :: int_size
  integer(kind=MPI_ADDRESS_KIND) :: sdisps(4), rdisps(4), disp0

  integer :: i, j
  integer :: gridr(3), coords(2)

  call mpi_init()
  call mpi_cart_create(MPI_COMM_WORLD, ndims, dims, periods, .true., cart_comm, rc)
  call mpi_comm_size(cart_comm, ntasks)
  call mpi_comm_rank(cart_comm, rank)

  !Print out process grid
  if (rank == 0) then
     write(*,*) "Process grid" 
     do i=0, dims(1) - 1
       do j=0, dims(2) -1
         coords = [i, j]
         call mpi_cart_rank(cart_comm, coords, gridr(j+1))
       end do
       write(*,*) gridr
     end do
  end if


  array = -1
  array(2:7, 2:7) = rank

  call mpi_type_vector(8, 1, 8, MPI_INTEGER, rowtype, rc)
  call mpi_type_contiguous(8, MPI_INTEGER, columntype, rc)
  call mpi_type_commit(rowtype, rc)
  call mpi_type_commit(columntype, rc)

  types(1:2) = rowtype
  types(3:4) = columntype

 ! call mpi_cart_shift(cart_comm, 0, 1, src, dest)
 ! call mpi_isend(array(2, 1), 1, types(1), src, 0, cart_comm, reqs(1))
 ! call mpi_irecv(array(1, 1), 1, types(1), src, 0, cart_comm, reqs(2))
 ! call mpi_isend(array(7, 1), 1, types(2), dest, 0, cart_comm, reqs(3))
 ! call mpi_irecv(array(8, 1), 1, types(2), dest, 0, cart_comm, reqs(4))
 ! call mpi_cart_shift(cart_comm, 1, 1, src, dest)
 ! call mpi_isend(array(1, 2), 1, types(3), src, 0, cart_comm, reqs(5))
 ! call mpi_irecv(array(1, 1), 1, types(3), src, 0, cart_comm, reqs(6))
 ! call mpi_isend(array(1, 7), 1, types(4), dest, 0, cart_comm, reqs(7))
 ! call mpi_irecv(array(1, 8), 1, types(4), dest, 0, cart_comm, reqs(8))

 ! call mpi_waitall(8, reqs, MPI_STATUSES_IGNORE)

  ! if (rank == 5) then
  !    write(*,*) 'Received data'
  !    do i=1, 8
  !       write(*,'(*(I3))') array(i, :)
  !    end do
  ! end if

 call mpi_get_address(array(1,1), disp0)
  ! calculate displacements with mpi_get_ddress
 call mpi_get_address(array(2,1), sdisps(1))
 call mpi_get_address(array(7,1), sdisps(2))
 call mpi_get_address(array(1,2), sdisps(3))
 call mpi_get_address(array(1,7), sdisps(4))

 call mpi_get_address(array(1,1), rdisps(1))
 call mpi_get_address(array(8,1), rdisps(2))
 call mpi_get_address(array(1,1), rdisps(3))
 call mpi_get_address(array(1,8), rdisps(4))

 do i = 1, 4
    sdisps(i) = sdisps(i) - disp0
    rdisps(i) = rdisps(i) - disp0
 end do

 ! call mpi_neighbor_alltoallw(MPI_BOTTOM, counts, sdisps, types, MPI_BOTTOM, counts, rdisps, types, cart_comm, rc)
 call mpi_neighbor_alltoallw(array, counts, sdisps, types, array, counts, rdisps, types, cart_comm, rc)

  if (rank == 5) then
     write(*,*) 'Received data'
     do i=1, 8
        write(*,'(*(I3))') array(i, :)
     end do
  end if
  call mpi_finalize()

end program  

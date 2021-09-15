## Heat equation solver with neighborhood collectives

If you are not familiar with the two dimensional heat equation, please have a look
for [basic description](https://github.com/csc-training/mpi-introduction/tree/main/heat-equation)
in "Parallel programming with MPI" exercise material.

Here, starting point is a working code parallelized over columns (in Fortran) or rows (in C/C++).

Implement the halo exchange in the `exchange()` routine in [cpp/core.cpp](cpp/core.cpp) 
or [fortran/core.F90](fortran/core.F90) with `MPI_Neighbor_alltoall`. In order to proceed,
you need to have the [usage of Cartesian communicator](README_cartesian.md) implemented.

1. Before start of the communication, the values at the real domain boundaries (*i.e* not the
   in the ghost region) need to be copied into a send buffer in the correct order. You may 
   allocate and deallocate the send and receive buffers within the `exchange` routine, or as 
   recommended as part of the "parallel" data structure.

2. Perform the halo exchange with a single call to `MPI_Neighbor_alltoall`. 

3. As the boundary values of neighboring processes are in the receive buffer, values need to be
   copied to the ghost region.

To build the code, please use the provided `Makefile` (by typing `make`). By default, Intel 
compiler is used, in order to use gcc type `make COMP=gnu`.

# Advanced MPI and parallel I/O

Exercise material and model answers for the CSC course "Advanced MPI and parallel I/O". 
The course is part of PRACE Training Center (PTC) activity at CSC.

## Exercises

[General instructions](exercise-instructions.md)


### Using own communicators

 - [Communicators and collectives](mpi/communicator)
 - [Cartesian grid process topology](mpi/cartesian-grid)
 - [Message chain with Cartesian communicator](mpi/message-chain-cartesian)
 - [(Bonus) Heat equation with cartesian communicator](heat-equation/README_cartesian.md)

### Persistent and neighborhood communication

 - [Message chain with persistent communication](mpi/message-chain-persistent)
 - [Neighborhood exchange](mpi/neighbor-exchange)
 - [(Bonus) Heat equation with neighborhood collectives](mpi/heat-equation/README_neighbor.md)

### Simple derived types

 - [Simple datatypes](mpi/simple-datatypes)
 - [(Bonus) 2D-decomposed heat equation](mpi/heat-equation/README_2d.md)

### Advanced derived types
 - [Communicating struct / derived type](mpi/struct-datatype)
 - [Modifying extent](mpi/datatype-extent)


### Parallel I/O

 - [Parallel I/O with Posix](parallel-io/posix)
 - [Simple MPI-IO](parallel-io/mpi-io)
 - [HDF5 example](parallel-io/hdf5)
 - [Bonus: Checkpoint + restart with MPI-IO](parallel-io/heat-restart)

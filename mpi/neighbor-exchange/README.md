## "Halo exchange" with neighborhood collectives

Consider a parallel program where processes are arranged in a one
dimensional chain. Each process stores local data in an array, but
reserves also "ghost layers" for communicating with neighboring
processes. Initially, the ghost data is zero, and real data consists
of sequence of integers:

```
Task 0:  0  0  1  2  3  0
Task 1:  0  4  5  6  7  0
Task 2:  0  8  9 10 11  0
...
```

Implement a program where each process sends the boundary values to
neighboring processes, so that after communication the arrays
(including ghost layers) look like:
```
...
Task 2:  7  8  9 10 11 12
Task 3: 11 12 13 14 15 16
Task 4: 15 16 17 18 19 20
...
```

1. Create a Cartesian communicator for the chain
2. Copy boundary values to a send buffer
3. Perform the communication with appropriate neighboordhood
   collective routine
4. Copy values from the receive buffer to the ghost layers.

Try both finite and periodic chains.

You may start from scratch or use [skeleton.cpp](skeleton.cpp) (or
[skeleton.F90](skeleton.F90) for Fortran) as a starting point.

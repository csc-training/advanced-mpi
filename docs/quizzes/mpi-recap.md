<!-- Adapted from material by EPCC -->
<!-- https://github.com/EPCCed/archer2-MPI-2020-05-14 -->

# MPI recap

Questions can be copy-pasted e.g. to HackMd.


1. What is MPI?

   A. the Message Passing interface
   B. the Miami Police Investigators
   C. the Minimal Polynomial instantiation
   D. the Millipede Podiatry institution
   E. a way of doing distributed memory parallel programming

   A.
   B.
   C.
   D.
   E.


Correct: A, E

2. How is a parallel MPI program executed?

   A. As a set of identical, independent processes
   B. Program starts serially, and then spawns and closes threads
   C. My MPI programs just crash :-(
   D. Each MPI task runs different program with different source code

   A.
   B.
   C.
   D.

Correct: A (even though in MPMD model D is also possible)


3. After initiating an MPI program with "mpiexec -n 4
./my_mpi-program", what does the call to MPI_Init() do?

   A. create the 4 parallel processes
   B. start program execution
   C. enable the 4 independent programs subsequently to communicate with each other
   D. create the 4 parallel threads

   A.
   B.
   C.
   D.


Correct: C

The standard MPI launchers (mpiexec, mpirun, srun, aprun etc.) create
multiple copies of your MPI executable, each being a separate OS
process; there are already multiple processes running before the call
to MPI_Init. These will happily run independently - what the MPI_Init 
call does is get them to talk to each other, which is required before
any user-initiated communications.

4. If you call MPI_Recv and there is no incoming message, what happens?

   A. the Recv fails with an error
   B. the Recv reports that there is no incoming message
   C. the Recv waits until a message arrives (potentially waiting forever)
   D. the Recv times out after some system specified delay (e.g. a few
minutes)

   A.
   B.
   C.
   D.


Correct: C

MPI is not fault tolerant and assumes all processes are alive all the
time. There are therefore no timeouts in MPI. The assumption is that
you have written a correct program and that a matching message will
(eventually) arrive. This means that an unmatched receive waits
forever.

5. If you call MPI_Send and there is no matching receive, which of the
   following are possible outcomes? 
   
A. the message disappears 
   B. the send fails with an error
   C. the send waits until a receive is posted (potentially waiting forever)
   D. the message is stored and delivered later on (if possible)
   E. the send times out after some system specified delay (e.g. a few minutes)
   F. the program continues execution regardless of whether the message
is received 

   A.
   B.
   C.
   D.
   E.
   F.

Correct: C, D

MPI_Send is blocking, so its completion *may* depend on the matching
receive. In practice, MPI will try to buffer small messages but,
rather than failing if there is insufficient buffer space, will switch
to using synchronous send for large messages. However, the threshold
to switch between the two is implementation dependent (and might even
change depending on how many processes you are running on) so you must
always handle the case that MPI_Send might be synchronous (or use
other MPI calls which will be discussed later on).


6. Which of the following are possible outputs from this piece of
    code run on 3 processes: 

```
printf("Welcome from rank %d\n“, rank);
printf("Goodbye from rank %d\n", rank);
```

   A.  
    ```
    Welcome from rank 0
    Welcome from rank 1
    Welcome from rank 2
    Goodbye from rank 0
    Goodbye from rank 1
    Goodbye from rank 2
	```
   B. 
    ```
	Welcome from rank 2
    Welcome from rank 1
    Goodbye from rank 0
    Goodbye from rank 1
    Goodbye from rank 2
    Welcome from rank 0
	```
   C.  
    ```
	Welcome from rank 2
    Goodbye from rank 2
    Welcome from rank 0
    Welcome from rank 1
    Goodbye from rank 1
    Goodbye from rank 0
	```
   D.  
    ```
	Welcome from rank 0
    Goodbye from rank 1
    Welcome from rank 2
    Goodbye from rank 0
    Welcome from rank 1
    Goodbye from rank 2
	```

   A.
   B.
   C.
   D.


Correct: A, C

The output from different MPI processes is interleaved in an
unpredictable way. The order in which two print statements appear on
the screen has, in general, nothing to do with when the printf
statements actually happened. Remember that the output from many 
separate programs running on many different computers is appearing on a
single screen - a lot of buffering etc. may take place between a print
statement executing and the output appearing on the screen in
particular, process x may execute a print statement before 
process Y, but the output could appear in the other order (i.e. Y
appears before X ).

However, the ordering from an individual process will be maintained,
so for a particular process "X" the "Goodbye" from X will always
appear after the "Welcome" from X. 

7. Which of the following statements do you agree with regarding this code:

```
for (i=0; i < size; i++)
 { 
    if (rank == i)
      {
        printf("Hello from rank %d\n", rank);
        j = 10*i;
      }
 }
```

   A. The for loop ensures the operations are in order: rank 0, then rank
1, ...
   B. The for loop ensures the operation are done in parallel across all processes
   C. The for loop is entirely redundant
   D. The final value of j will be equal to 10*(size-1)

   A.
   B.
   C.
   D.


Correct: C

 Remember that every MPI process is executing the entire piece of code
 so each process only ever executes the if statement once when `rank ==
 i` The for loop is therefore
redundant, the above code produces exactly the same result as:

```
  printf("Hello from rank %d\n", rank);
  j = 10*rank;
```


8. How can boundary MPI tasks *e.g.* in communication chain be treated?

   A. Using `MPI_PROC_NULL`
   B. Using `MPI_ANY_SOURCE`
   C. By putting MPI calls into "if-else" structures
   D. With the help of "modulo" operations

   A.
   B.
   C.
   D.

Correct: A, C, D

C is generally not recommended as code becomes often
cluttered. MPI_PROC_NULL is useful with finite chains where boundary
domains only send or receive. In periodic chains, modulo operations can
be used for wrapping destination and source ranks. (MPI_Cart_create is
even more convenient in most cases but not discussed in this course).


9. Which of the following statements about Collective communication
   are correct?

   A. All the processes participate in pairwise communication
   B. Every process sends a message to a specific process
   C. All the MPI tasks within a communicator communicate along
      the chosen pattern
   D. Every process receives messages from every other process
  
   A.
   B.
   C.
   D.

Correct: C

B. and D. may happen with some collective call over MPI_COMM_WORLD,
but are not true for collective communication in general


10. What is the outcome of the following code snippet when run with 4 processes?
   ```fortran
   a(:) = my_id
   call mpi_gather(a, 2, MPI_INTEGER, aloc, 2, MPI_INTEGER, 3, MPI_COMM_WORLD, rc)
   if (my_id==3) print *, aloc(:)
   ```

   A. "0 1 2 3"
   B. "2 2 2 2 2 2 2 2"
   C. "0 0 1 1 2 2 3 3"
   D. "0 1 2 3 0 1 2 3"
  
   A.
   B.
   C.
   D.

Correct: C
  
11. What is the outcome of the following code snippet when run with 8
   processes, i.e. on ranks 0, 1, 2, 3, 4, 5, 6, 7
   ```c
   if (rank % 2 == 0) { // Even processes
      MPI_Allreduce(&rank, &evensum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
      if (0 == rank) printf("evensum = %d\n", evensum);
   } else { // odd processes
      MPI_Allreduce(&rank, &oddsum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
      if (1 == rank) printf("oddsum = %d\n", oddsum);
   }
   ```
   
   A. evensum = 16, oddsum = 12
   B. evensum = 28, oddsum = 28
   C. evensum = 12, oddsum = 16
   D. evensum = 6, oddsum = 2

   A.
   B.
   C.
   D.

Correct: B

This is the closest we have to a trick question, written to make it
look like the correct answer is C!

Although the reduction operation is called from different lines by
different processes, they are an participating in **the same**
reduction as the communicator is MPI_COMM_WORLD. 
As a result, every process computes the sum of all the ranks which is
28, the only difference is that even ranks store the result in
evensum, odd ranks in oddsum. 

Although you often have all processes executing a reduction from the
same line of code, there is no requirement to do this. In fact, as a
library, MPI has no way of knowing where the reduction call was made,
all it knows is that, as it is across MPI_COMM_WORLD, it needs to wait
until all processes have called the routine. If you wanted to compute even
and odd rank sums separately, you would need to split MPI_COMM_WORLD
into two sub-communicators, one for even ranks and the other for odd,
and call then reduction on this subcommunicaior.
	
  

12. What is outcome of the following code snippet?
   ```cpp
   if (0 == myid) {
      int a = 4;
	  MPI_Isend(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &req);
	  a = 6;
	  MPI_Wait(&req, MPI_STATUS_IGNORE);
   } else if (1 == myid) {
      int a;
	  MPI_Irecv(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &req);
	  std::cout << "a is: " << a << std::endl;
      MPI_Wait(&req, MPI_STATUS_IGNORE);
  }
  ```
  
   A. 4
   B. 6
   C. Segmentation fault
   D. Not well defined

   A.
   B.
   C.
   D.


Correct: D

  The input buffer of `Isend` should not be modified before the
  communication is finalized with `Wait` or `Test` etc. Also, the
  output buffer of `Irecv` should not be read before receive is
  finalized. In practice, if the data was not received yet before
  printout, the outcome is 0 (or some random value if 
  compiler did not initialize `a` to 0).  If the communication happened
  to take place before rank 0 modified `a`, outcome could be 4, or 6
  if rank 0 managed to modify `a` before communication was realized.

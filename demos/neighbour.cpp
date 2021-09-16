#include <cstdio>
#include <vector>
#include <iostream>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank, ntasks;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    if (ntasks != 9) {
       std::cout << "This demo works only with 9 MPI tasks" << std::endl;
       MPI_Abort(MPI_COMM_WORLD, -1);
    }

    MPI_Comm cart_comm;
    int ndims = 2;
    int dims[2] = {3, 3};
    int periods[2] = {0, 1};
    MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, 0, &cart_comm); 
    MPI_Comm_rank(cart_comm, &rank);

    // Print out process grid
    if (0 == rank) {
        std::cout << "Process grid" << std::endl;
        for (int i=0; i < dims[0]; i++) {
           for (int j=0; j < dims[1]; j++) {
              int r;
              int coords[2] = {i, j};
              MPI_Cart_rank(cart_comm, coords, &r);
              std::cout << r << "  ";
           }
           std::cout << std::endl;
        }
        std::cout << std::flush;
    }

    MPI_Barrier(cart_comm);
    
    int neighbors = 4;
    std::vector<int> sendbuf(2, rank);
    std::vector<int> recvbuf(2*neighbors, -1);

    MPI_Neighbor_allgather(sendbuf.data(), 2, MPI_INT,
                           recvbuf.data(), 2, MPI_INT,
                           cart_comm);

   
    // Print out the result
    for (int r=0; r < ntasks; r++) {
        MPI_Barrier(cart_comm);
        if (rank == r) {
           std::cout << "task " << rank << ": ";
           for (auto i: recvbuf) 
             std::cout << i << " ";
           std::cout << std::endl << std::flush;
        }
        MPI_Barrier(cart_comm);
    }

    MPI_Finalize();
}

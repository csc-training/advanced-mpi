#include <cstdio>
#include <mpi.h>

int main(int argc, char **argv)
{
    int world_rank, rank, ntasks;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int ndims = 2;
    int dims[2] = {0, 0};

    int rc = MPI_Dims_create(ntasks, ndims, dims);
    if (0 == world_rank)
        printf("Decomposing %d ntasks into %d x %d grid\n", ntasks, dims[0], dims[1]);

    int periods[2] = {0, 1};
    MPI_Comm cart_comm;
    MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, 1, &cart_comm); 

    int coords[2];
    int nup, ndown, nleft, nright;
    MPI_Comm_rank(cart_comm, &rank);
    MPI_Cart_coords(cart_comm, rank, 2, coords);
    MPI_Cart_shift(cart_comm, 0, 1, &nup, &ndown);
    MPI_Cart_shift(cart_comm, 1, 1, &nleft, &nright);
    printf("Coords of %d (%d): %d, %d, neighbours %d %d %d %d\n", 
           rank, world_rank, coords[0], coords[1], nup, ndown, nleft, nright);

    MPI_Finalize();

}

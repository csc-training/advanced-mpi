#include <stdio.h>
#include <iostream>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank, ntasks;
    int array[8][8];
    int buf[8][8];
    MPI_Datatype columntype, rowtype, types[4];
    MPI_Comm cart_comm;
    int ndims = 2;
    int dims[2] = {3, 3};
    int periods[2] = {0, 1};
    int coords[2];

    int counts[4] = {1, 1, 1, 1};
    MPI_Aint sdisps[4];
    MPI_Aint rdisps[4];
    
    int i, j;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

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

    // Initialize arrays
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            array[i][j] = -1;
        }
    }
    for (i = 1; i < 7; i++) {
        for (j = 1; j < 7; j++) {
            array[i][j] = rank;
        }
    }

    // Create datatypes
    MPI_Type_vector(8, 1, 8, MPI_INT, &columntype);
    MPI_Type_commit(&columntype);
    MPI_Type_contiguous(8, MPI_INT, &rowtype);
    MPI_Type_commit(&rowtype);

    types[0] = rowtype;
    types[1] = rowtype;
    types[2] = columntype;
    types[3] = columntype;

    // Determine displacements with C pointer arithmetic
    MPI_Aint disp0; 
    MPI_Get_address(&array[0][0], &disp0);
    MPI_Get_address(&array[1][0], &sdisps[0]); // second row
    MPI_Get_address(&array[6][0], &sdisps[1]); // second last row
    MPI_Get_address(&array[0][1], &sdisps[2]); // second column
    MPI_Get_address(&array[0][6], &sdisps[3]); // second last column

    MPI_Get_address(&array[0][0], &rdisps[0]); // first row
    MPI_Get_address(&array[7][0], &rdisps[1]); // last row
    MPI_Get_address(&array[0][0], &rdisps[2]); // first column
    MPI_Get_address(&array[0][7], &rdisps[3]); // last column

    for (int i=0; i < 4; i++) {
        sdisps[i] -= disp0;
        rdisps[i] -= disp0;
    }

    MPI_Neighbor_alltoallw(&array[0][0], counts, sdisps, types, &array[0][0], counts, rdisps, types, cart_comm);

    // Print out the result
    if (rank == 4) {
        printf("Received data in 4\n");
            for (i = 0; i < 8; i++) {
                for (j = 0; j < 8; j++) {
                    printf("%3d", array[i][j]);
                }
                printf("\n");
            }
    }

    MPI_Type_free(&rowtype);
    MPI_Type_free(&columntype);
    MPI_Finalize();

    return 0;
}

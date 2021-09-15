#include <cstdio>
#include <vector>
#include <mpi.h>

void print_buffers(std::vector<int> &buffer, MPI_Comm comm);

int main(int argc, char *argv[])
{
    int myid, ntasks;
    constexpr int size = 4;
    std::vector<int> data(size + 2, 0);
    std::vector<int> sendbuf(2), recvbuf(2);
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    MPI_Comm cart_comm;
    int ndims = 1;
    int dims[1] = {ntasks};
    int periods[1] = {1};

    MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, 1, &cart_comm);
    int cart_id;
    MPI_Comm_rank(cart_comm, &cart_id);
    
    // Inner part of local data
    for (int i = 1; i < size + 1; i++) {
      data[i] = cart_id * size + (i - 1);
    }

    // Print out initial data
    if (0 == cart_id) 
      printf("Initial data\n");
    print_buffers(data, cart_comm);

    // Copy data to send buffer
    sendbuf[0] = data[1];
    sendbuf[1] = data[size];

    // Communicate with neighbor
    MPI_Neighbor_alltoall(sendbuf.data(), 1, MPI_INT, recvbuf.data(),
                          1, MPI_INT, cart_comm);
    

    // Copy receive buffer to "halo"
    data[0] = recvbuf[0];
    data[size + 1] = recvbuf[1];

    // Print out results
    if (0 == cart_id) 
      printf("Final data\n");
    print_buffers(data, cart_comm);

    MPI_Finalize();
}

void print_buffers(std::vector<int> &buffer, MPI_Comm comm)
{
  int rank, ntasks;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &ntasks);

  const int buffersize = buffer.size();
  std::vector<int> printbuffer(ntasks * buffersize);

  MPI_Gather(buffer.data(), buffersize, MPI_INT,
             printbuffer.data(), buffersize, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    for (int j = 0; j < ntasks; j++) {
      printf("Task %i:", j);
      for (int i = 0; i < buffersize; i++) {
        printf(" %2i", printbuffer[i + buffersize * j]);
      }
      printf("\n");
    }
    printf("\n");
  }
}

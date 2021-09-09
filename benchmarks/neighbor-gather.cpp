#include <cstdio>
#include <vector>
#include <mpi.h>

int main(int argc, char **argv)
{
    int world_rank, rank, ntasks;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    struct times {
      int count;
      double mean_time;
      double max_time;
    };

    std::vector<times> sr_times;  // Times for sendrecv
    std::vector<times> nr_times;  // Times for neighborhood collective 

    constexpr int ndims = 3;
    int dims[3] = {0, 0, 0};

    MPI_Dims_create(ntasks, ndims, dims);
    if (0 == world_rank)
      printf("# %d ntasks in %d x %d x %d grid\n", ntasks, dims[0], dims[1], dims[2]);

    int periods[3] = {0, 0, 0};
    MPI_Comm cart_comm;
    MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, 1, &cart_comm); 
    MPI_Comm_rank(cart_comm, &rank);

    // Determine neighbors
    int nghbrs[ndims][2];
    for (int i=0; i < ndims; i++) {
      MPI_Cart_shift(cart_comm, i, 1, &nghbrs[i][0], &nghbrs[i][1]);
    }

    // All gather operation
    constexpr int max_repeat = 50;
    for (int count = 128; count < 1024*1024; count *= 2) {
      std::vector<double> sendbuf (count, rank);
      std::vector<double> recvbuf (count * ndims * 2, 0.0);
      std::vector<double> recvbuf2 (count * ndims * 2, 0.0);

      MPI_Barrier(cart_comm);

      double mean_time, max_time;
      // Communication with normal sendrecv
      double t0 = MPI_Wtime();
      for (int nrepeat = 0; nrepeat < max_repeat; nrepeat ++) {
	for (int i=0; i < ndims; i++) {
	  // Receive from up, send down
	  int recvslot = (0 + i * 2) * count;
	  MPI_Sendrecv(sendbuf.data(), count, MPI_DOUBLE, nghbrs[i][1], 1, 
		       recvbuf.data() + recvslot, count, MPI_DOUBLE, nghbrs[i][0], 1, 
		       cart_comm, MPI_STATUS_IGNORE);
	  // Receive from down, send up
	  recvslot = (1 + i * 2) * count;
	  MPI_Sendrecv(sendbuf.data(), count, MPI_DOUBLE, nghbrs[i][0], 1, 
		       recvbuf.data() + recvslot, count, MPI_DOUBLE, nghbrs[i][1], 1, 
		       cart_comm, MPI_STATUS_IGNORE);
	}
      }
      double t1 = MPI_Wtime();
      double ttot = t1 - t0;
      MPI_Reduce(&ttot, &mean_time, 1, MPI_DOUBLE, MPI_SUM, 0, cart_comm);
      MPI_Reduce(&ttot, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, cart_comm);

      mean_time /= (ntasks * max_repeat);
      max_time /= (max_repeat);
      sr_times.push_back({count, mean_time, max_time});

      // Communication with neighborhood collective
      t0 = MPI_Wtime();
      for (int nrepeat = 0; nrepeat < max_repeat; nrepeat ++) {
	MPI_Neighbor_allgather(sendbuf.data(), count, MPI_DOUBLE, 
			       recvbuf2.data(), count, MPI_DOUBLE, cart_comm);
	}   
      t1 = MPI_Wtime();
      ttot = t1 - t0;
      MPI_Reduce(&ttot, &mean_time, 1, MPI_DOUBLE, MPI_SUM, 0, cart_comm);
      MPI_Reduce(&ttot, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, cart_comm);

      mean_time /= (ntasks * max_repeat);
      max_time /= (max_repeat);
      nr_times.push_back({count, mean_time, max_time});

      // Check result
      if (recvbuf != recvbuf2)
	printf("Something is wrong %4.2f %4.2f %4.2f %4.2f!!!\n",
	       recvbuf[0], recvbuf[count], recvbuf2[0], recvbuf2[count]);


    }

    // Print out results
    if (0 == rank) {
      printf("#            Sendrecv                         Neighbor_allgather   \n");
      printf("# Bytes   mean bandwidth  min bandwidth   mean bandwidth  min bandwidth (MB / s)\n");
      for (int n=0; n < sr_times.size(); n++) {
	auto bytes = sr_times[n].count * sizeof(double);
	auto mean_bw = 1.0e-6 * bytes / sr_times[n].mean_time; 
	auto min_bw = 1.0e-6 * bytes / sr_times[n].max_time; 
	printf("%7d    %8.3f        %8.3f", bytes, mean_bw, min_bw);
	mean_bw = 1.0e-6 * bytes / nr_times[n].mean_time; 
	min_bw = 1.0e-6 * bytes / nr_times[n].max_time; 
	printf("          %8.3f         %8.3f\n", bytes, mean_bw, min_bw);
      }

    }    

    MPI_Finalize();
}

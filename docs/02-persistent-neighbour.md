---
title:  Additional communication schemes
author: CSC Training
date:   2021
lang:   en
---


# Persistent communication {.section}

# Persistent communication

- Often a communication with same argument list is repeatedly executed
- It may be possible to optimize such pattern by persistent communication requests
    - Can be thought as a ”communication port”
- Usage:
    - Create requests: `MPI_Send_init` & `MPI_Recv_init`
        - Initiation and starting of communication separated in addition to communication and completion
    - Start communication: `MPI_Start` / `MPI_Startall`
    - Complete communication: `MPI_Wait` / `MPI_Waitall`
- Recently published (June 2021) MPI 4.0 includes also persistent
  collectives
    - Not supported by all implementations yet

# Persistent point-to-point communication

```c
MPI_Request recv_obj, send_obj;
...
// Initialize send/request objects
MPI_Recv_init(buf1, cnt, MPI_DOUBLE, src, tag, MPI_COMM_WORLD, &recv_obj);
MPI_Send_init(buf2, cnt, MPI_DOUBLE, dst, tag, MPI_COMM_WORLD, &send_obj);
for (i=1; i<BIGNUM; i++){
// Start communication described by recv_obj and send_obj
    MPI_Start(&recv_obj);
    MPI_Start(&send_obj);
    // Do work, e.g. update the interior domains 
    ...
    // Wait for send and receive to complete
    MPI_Wait(&send_obj, MPI_STATUS_IGNORE);
    MPI_Wait(&recv_obj, MPI_STATUS_IGNORE);
}
//Clean up the requests
MPI_Request_free (&recv_obj); MPI_Request_free (&send_obj);
```

# Neighborhood collectives {.section}

# Neighborhood collectives

- Neighborhood collectives build on top of process topologies
- Provide optimization possibilities for MPI library for communication
  patterns involving "neighbors"
- Similar to ordinary collectives, all tasks within a communicator
  need to call the routine
- Possible to have multidimensional halo-exchange with a single MPI call

# Cartesian neighborhood collectivesg
  

// Main solver routines for heat equation solver

#include <mpi.h>

#include "heat.hpp"

// Exchange the boundary values
void exchange(Field& field, const ParallelData parallel)
{

    MPI_Datatype types[4] = {parallel.rowtype, parallel.rowtype,
                             parallel.columntype, parallel.columntype};
    int counts[4] = {1, 1, 1, 1};
    MPI_Aint sdisps[4];
    MPI_Aint rdisps[4];

    sdisps[0] = (field.ny + 2) * sizeof(double); // Second row
    sdisps[1] = field.nx * (field.ny + 2) * sizeof(double); // Second last row
    sdisps[2] = 1 * sizeof(double); // Second column
    sdisps[3] = field.ny * sizeof(double); // Second last column

    rdisps[0] = 0; // First row
    rdisps[1] = (field.nx + 1) * (field.ny + 2) * sizeof(double); // Last row
    rdisps[2] = 0; // First column
    rdisps[3] = (field.ny + 1) * sizeof(double); // Last column

    MPI_Neighbor_alltoallw(field.temperature.data(), counts, sdisps, types,
                           field.temperature.data(), counts, rdisps, types,
                           parallel.comm);
}

// Update the temperature values using five-point stencil */
void evolve(Field& curr, const Field& prev, const double a, const double dt)
{

  // Compilers do not necessarily optimize division to multiplication, so make it explicit
  auto inv_dx2 = 1.0 / (prev.dx * prev.dx);
  auto inv_dy2 = 1.0 / (prev.dy * prev.dy);

  // Determine the temperature field at next time step
  // As we have fixed boundary conditions, the outermost gridpoints
  // are not updated.
  for (int i = 1; i < curr.nx + 1; i++) {
    for (int j = 1; j < curr.ny + 1; j++) {
            curr(i, j) = prev(i, j) + a * dt * (
	       ( prev(i + 1, j) - 2.0 * prev(i, j) + prev(i - 1, j) ) * inv_dx2 +
	       ( prev(i, j + 1) - 2.0 * prev(i, j) + prev(i, j - 1) ) * inv_dy2
               );
    }
  }

}

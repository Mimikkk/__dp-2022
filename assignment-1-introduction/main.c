#include <mpi.h>
#include <stdio.h>

int main() {
  int size, rank, len;
  char processor[100];
  MPI_Init(NULL, NULL);

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(processor, &len);

  printf("Hello world: %d of %d na (%s)\n", rank, size, processor);
  MPI_Finalize();
}
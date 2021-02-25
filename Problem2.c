#include <stdio.h>
#include <mpi.h>

int main( int argc, char *argv[] )
{
  int rank, size;
  double start, end;
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  printf( "I am %d of %d\n", rank, size );
  int token;

  start = MPI_Wtime();
  if (rank != 0) {
    MPI_Recv(&token, 1, MPI_INT, (rank - 1) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process %d received token %d from process %d\n", rank, token, (rank - 1) % size);
  } else {
    // Set the token's value if you are process 0
    token = -10;
  }
  MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
  // Now process 0 can receive from the last process.
  if (rank == 0) {
    MPI_Recv(&token, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    end = MPI_Wtime();
    printf("Execution time %f\n", end-start);
    printf("Process %d received token %d from process %d\n", rank, token, size - 1);
  }
  MPI_Finalize();
  // return 0;
}

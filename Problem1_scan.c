#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[])   {

  int n = 8;
  int a[8] = {3, 1, 7, 1, 5, 1, 5, 3};
  int b[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  // int i = 0;
  int rank,size;
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size); 
  // int iret;
  MPI_Scan(&a[rank], &b[rank], 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  printf(" Process of rank %d has array b=[%d,%d,%d,%d,%d,%d,%d,%d]\n",
	 rank, b[0],b[1],b[2],b[3],b[4],b[5],b[6],b[7]);  
  MPI_Finalize();
  return 0;
}





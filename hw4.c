#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
  int nthreads;
  int array[32]={0};
  //nthreads = omp_get_num_threads();
  /* Fork a team of threads giving them their own copies of variables */
#pragma omp parallel private(nthreads)
  {
  /* Obtain thread number */
    int tid = omp_get_thread_num();
    /* Obtain total threads  */
    nthreads = omp_get_num_threads();
    int i; 
       for(i=tid; i<32; i += nthreads){
	array[i] = 0;
    } /* All threads join master thread and disband */
  }
  printf("done with part a\n");

#pragma omp parallel private(nthreads)
  {
    /* Obtain thread number */
    int tid = omp_get_thread_num();

    /* Only master thread does this */
    nthreads = omp_get_num_threads();
 /* All threads join master thread and disband */
   int i;
   for (i=tid; i<32; i+=nthreads) {
     array[i]=5*i;
   }
  }

  printf("dont with part b\n");

  int oddSum, oddCount, lSum, lCount;
  int i, tid;

#pragma omp parallel private(i, tid, lSum, lCount, nthreads) shared(oddSum,oddCount)
  {
    /* Obtain thread number */
    tid = omp_get_thread_num();
    nthreads = omp_get_num_threads();
    lSum = 0;
    lCount = 0;
    #pragma omp for schedule (static, 1)
    for (i=0; i<32; i++) {
      //printf("FOR tid:%d, i:%d, array[i]:%d\n", tid, i, array[i]);
      if (array[i] % 2 == 1) {
	//printf("IF tid:%d, i:%d, array[i]:%d\n", tid, i, array[i]);
        lSum += array[i];
        lCount += 1;
      }
    }
  #pragma omp critical
  {
    oddSum += lSum;
    //#pragma omp critical
    oddCount += lCount;
  }

    /* Only master thread does this */
   #pragma omp barrier
    if (tid == 0) {
      //nthreads = omp_get_num_threads();

      // #pragma omp for schedule (static, 4)
      printf("From master thread:%d\n", tid, i);
      /*for (i=0; i<32; i++) {
        printf("a[%d]= %d\n", i, array[i]);
	}*/
      printf("Number of Odd:%d, Sum of odd:%d\n", oddCount, oddSum);
    }
  }
 /* All threads join master thread and disband */
  //printf("Number of odds:%d\n", oddCount);
  return 0;
}



#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
  int nthreads;
  int array[32]={0};
  omp_set_num_threads(8);
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
      if (array[i] % 2 == 1) {
        lSum += array[i];
        lCount += 1;
      }
    }
  #pragma omp critical
  {
    oddSum += lSum;
    oddCount += lCount;
  }

    /* Only master thread does this */
   #pragma omp barrier
    if (tid == 0) {
      //nthreads = omp_get_num_threads();

      // #pragma omp for schedule (static, 4)
      printf("From master thread:%d\n", tid, i);
      printf("Number of Odd:%d, Sum of odd:%d\n", oddCount, oddSum);
    }
  }
 /* All threads join master thread and disband */
  return 0;
}



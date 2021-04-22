#include <stdio.h>
#include <cuda.h>
#include <curand.h>
#include <curand_kernel.h>


//this is the function that finds the min within the matrix
__global__ void getminimum(unsigned *da, unsigned* minValue){
  int i = threadIdx.x * blockDim.y + threadIdx.y;
  atomicMin(minValue, da[i]);
}



//fill matrix with random nums
__global__ void generateRandomMatrix(unsigned *da, float x, int n)
{
  curandState_t state;
  curand_init(clock64(), 0, 0, &state);
  int i = threadIdx.x * blockDim.y + threadIdx.y;
  da[i] = curand(&state) % 100 + i;
}


//set min as max int value
__global__ void makeMax(unsigned *min){
   min[0] = INT_MAX; 
}


__global__ void square ( unsigned *matrix, unsigned *result, unsigned matrixsize) {
  unsigned id = threadIdx.x * blockDim.y + threadIdx.y;
  for (unsigned jj = 0; jj < matrixsize; jj++) {
    for (unsigned kk = 0; kk < matrixsize; kk++) {
      result[id * matrixsize + jj] += matrix[id * matrixsize + kk] * matrix[kk * matrixsize + jj]; 
     }
   }
}


__global__ void initializeResult(unsigned *result, unsigned size){
  for(unsigned i = 0; i < size; i++){
    result[i] = 0;
 }
}



//PART 1 findMIN
__host__ void findMin(unsigned *matrix, dim3 block){
  unsigned *min, *C_min;                              //store the min number
  min = (unsigned *)malloc(1 * sizeof(unsigned));     //allocate space for min
  cudaMalloc((void **)&C_min, 1 * sizeof(unsigned));  //allocate space for device copy
  cudaMemcpy(C_min, min, 1 * sizeof(unsigned), cudaMemcpyHostToDevice);  //copy input to device 
  makeMax<<<1,1>>>(C_min);                            //set min value as a MAX for initial
  getminimum<<<1, block>>>(matrix, C_min);             //find the min within matrix
  cudaMemcpy(min, C_min, 1 * sizeof(unsigned), cudaMemcpyDeviceToHost); //copy back to host
  printf("Min Value is %d\n", min[0]);                  //print min
  free(min);
  cudaFree(C_min);
}




#define N 16
//PART 2 findSquare
__host__ void findSquare(unsigned *matrix){
  unsigned *result, *hresult;                                  //store the square
  hresult = (unsigned *)malloc(N * N  * sizeof(unsigned));     //allocate space for square
  cudaMalloc(&result, N * N  * sizeof(unsigned));              //allocate space for device copy
  initializeResult<<<1, N>>>(result, N);                       
  square<<<1, N>>>(matrix, result, N);                         //set square for n 
  cudaMemcpy(hresult, result, N * N * sizeof(unsigned), cudaMemcpyDeviceToHost);  //copy back to host
  printf("Resulting square Matrix\n");             //print square
  for(unsigned ii = 0; ii < N; ++ii){
    for(unsigned jj = 0; jj < N; ++jj){
      //printf("%2d ", hresult[ii * N + jj]);
    }
    printf("\n");
 }
}


int main() {
    dim3 block(N, N, 1);
    unsigned *matrix, *hmatrix;                         //hold the matrix
    cudaMalloc(&matrix, N * N * sizeof(unsigned));      //alloc matrix
    hmatrix = (unsigned *)malloc(N * N * sizeof(unsigned));  //alloc matrix
    generateRandomMatrix<<<1, block>>>(matrix, 5, 30);  //fill matrix with random numbers
    cudaMemcpy(hmatrix, matrix, N * N * sizeof(unsigned), cudaMemcpyDeviceToHost);
    printf("starting matrix\n");
      for (unsigned ii = 0; ii < N; ++ii) {
        for (unsigned jj = 0; jj < N; ++jj) {
           // printf("%2d ", hmatrix[ii * N + jj]);
        }
        printf("\n");
   }
   findMin(matrix, block);  
   findSquare(matrix);
   return 0;
}

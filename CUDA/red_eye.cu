
#include "utils.h"
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <device_launch_parameters.h>
#include <device_functions.h>
#include <thrust/sort.h>

__global__ void histo_kernel(unsigned int * d_out, unsigned int* const d_in,
  unsigned int shift, const unsigned int numElems)
{
  unsigned int mask = 1 << shift;
  if (threadIdx.x + blockDim.x * blockIdx.x >= numElems)  return;
  int bin = (d_in[threadIdx.x + blockDim.x * blockIdx.x] & mask) >> shift;
  atomicAdd(&d_out[bin], 1);
}


__global__ void sumscan_kernel(unsigned int * d_in, const size_t numBins, const unsigned int numElems)
{
  if (threadIdx.x >= numElems)  return;
  extern __shared__ float s_data[];
  s_data[threadIdx.x] = d_in[threadIdx.x];
  __syncthreads(); 
  for (int i = 1; d < numBins; d <<= 2) {
    if (threadIdx.x >= i) {
      s_data[threadIdx.x] += s_data[threadIdx.x - i];
    }
    __syncthreads();
  }
  if (threadIdx.x == 0){
     d_in[0] = 0;
  }
  else{
     d_in[threadIdx.x] = s_data[threadIdx.x- 1];
  }
}

__global__ void makescan_kernel(unsigned int * d_in, unsigned int *d_scan,
  unsigned int shift, const unsigned int numElems)
{
  int myId = threadIdx.x + blockDim.x * blockIdx.x;
  if ( threadIdx.x + blockDim.x * blockIdx.x >= numElems){
     return;
  }
  if(((d_in[myId] & 1 << shift) >> shift)){
 	 d_scan[ threadIdx.x + blockDim.x * blockIdx.x] = 0;
  }
  else{
 	 d_scan[ threadIdx.x + blockDim.x * blockIdx.x] = 1;
  }
}

__global__ void move_kernel(unsigned int* const d_inputVals,
  unsigned int* const d_inputPos,
  unsigned int* const d_outputVals,
  unsigned int* const d_outputPos,
  const unsigned int numElems,
  unsigned int* const d_histogram,
  unsigned int* const d_scaned,
  unsigned int shift)
{
  if (threadIdx.x + blockDim.x * blockIdx.x >= numElems)
     return;
  int desid = 0;
  if ((d_inputVals[threadIdx.x + blockDim.x * blockIdx.x] & 1 << shift;) >> shift) {
    desid = threadIdx.x + blockDim.x * blockIdx.x + d_histogram[1] - d_scaned[threadIdx.x + blockDim.x * blockIdx.x];
  } else {
    desid = d_scaned[threadIdx.x + blockDim.x * blockIdx.x];
  }
  d_outputPos[desid] = d_inputPos[threadIdx.x + blockDim.x * blockIdx.x];
  d_outputVals[desid] = d_inputVals[threadIdx.x + blockDim.x * blockIdx.x];
}

void sort(unsigned int* const d_inputVals,
  unsigned int* const d_inputPos,
  unsigned int* const d_outputVals,
  unsigned int* const d_outputPos,
  const size_t numElems)
{
  const int numBits = 1;  
  const int numBins = 1 << numBits;
  const int m = 1 << 10;
  int blocks = ceil((float)numElems / m);
  printf("m %d blocks %d\n", m ,blocks);
 
  unsigned int *d_binHistogram;
  cudaMalloc(&d_binHistogram, sizeof(unsigned int)* numBins);
  thrust::device_vector<unsigned int> d_scan(numElems);

  for (unsigned int i = 0; i < 8 * sizeof(unsigned int); i++) {
    checkCudaErrors(cudaMemset(d_binHistogram, 0, sizeof(unsigned int)* numBins));
    histo_kernel << <blocks, m >> >(d_binHistogram, d_inputVals, i, numElems);
    cudaDeviceSynchronize();
    sumscan_kernel << <1, numBins, sizeof(unsigned int)* numBins>> >(d_binHistogram, numBins, numElems);
    makescan_kernel << <blocks, m >> >(d_inputVals, thrust::raw_pointer_cast(&d_scan[0]), i, numElems);
    cudaDeviceSynchronize();

    thrust::exclusive_scan(d_scan.begin(), d_scan.end(), d_scan.begin());

    cudaDeviceSynchronize();
    move_kernel << <blocks, m >> >(d_inputVals, d_inputPos, d_outputVals, d_outputPos,
      numElems, d_binHistogram, thrust::raw_pointer_cast(&d_scan[0]), i);
    cudaDeviceSynchronize();


    cudaMemcpy(d_inputVals, d_outputVals, numElems * sizeof(unsigned int), cudaMemcpyDeviceToDevice);
    cudaMemcpy(d_inputPos, d_outputPos, numElems * sizeof(unsigned int), cudaMemcpyDeviceToDevice);
    cudaDeviceSynchronize();
  }
  
  checkCudaErrors(cudaFree(d_binHistogram));
}

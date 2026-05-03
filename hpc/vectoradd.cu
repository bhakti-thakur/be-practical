// cell 1: !nvidia-smi

// cell 2: 
%%writefile vectoradd.cu
%%cu
#include <iostream>
#include <ctime>
using namespace std;

// CUDA Kernel
__global__ void vector_add(int* A, int* B, int* C, int n) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid < n)
        C[tid] = A[tid] + B[tid];
}

// Main
int main() {
    int n;
    cout << "Enter vector size: ";
    cin >> n;
    size_t bytes = n * sizeof(int);

    // Host arrays
    int *A = new int[n], *B = new int[n], *C = new int[n];
    srand(time(0));
    for (int i = 0; i < n; i++) { A[i] = rand() % 1000; B[i] = rand() % 1000; }

    // Device arrays
    int *dA, *dB, *dC;
    cudaMalloc(&dA, bytes);
    cudaMalloc(&dB, bytes);
    cudaMalloc(&dC, bytes);

    cudaMemcpy(dA, A, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(dB, B, bytes, cudaMemcpyHostToDevice);

    int threads = 256;
    int blocks = (n + threads - 1) / threads;

    // Timing
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start);
    vector_add<<<blocks, threads>>>(dA, dB, dC, n);
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float ms = 0;
    cudaEventElapsedTime(&ms, start, stop);
    cout << "GPU vector addition time: " << ms << " ms\n";

    cudaMemcpy(C, dC, bytes, cudaMemcpyDeviceToHost);

    // Verify first 5
    cout << "Sample (first 5): ";
    for (int i = 0; i < 5 && i < n; i++)
        cout << A[i] << "+" << B[i] << "=" << C[i] << "  ";
    cout << "\n";

    delete[] A; delete[] B; delete[] C;
    cudaFree(dA); cudaFree(dB); cudaFree(dC);
    cudaEventDestroy(start); cudaEventDestroy(stop);
    return 0;
}

// cell 3: !nvcc vectoradd.cu -o vectoradd && ./vectoradd
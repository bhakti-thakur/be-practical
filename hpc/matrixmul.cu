// cell 1: !nvidia-smi

// cell 2: 
%%writefile matrixmul.cu
#include <iostream>
#include <cuda.h>
using namespace std;

#define BLOCK_SIZE 16

// CUDA Kernel
__global__ void matmul(float* A, float* B, float* C, int N) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    if (row < N && col < N) {
        float sum = 0;
        for (int k = 0; k < N; k++)
            sum += A[row * N + k] * B[k * N + col];
        C[row * N + col] = sum;
    }
}

// Main 
int main() {
    int N;
    cout << "Enter matrix size (N for NxN): ";
    cin >> N;
    size_t bytes = N * N * sizeof(float);

    // Host arrays
    float *hA = new float[N*N], *hB = new float[N*N], *hC = new float[N*N];
    for (int i = 0; i < N * N; i++) { hA[i] = rand() % 10; hB[i] = rand() % 10; }

    // Device arrays
    float *dA, *dB, *dC;
    cudaMalloc(&dA, bytes);
    cudaMalloc(&dB, bytes);
    cudaMalloc(&dC, bytes);
    cudaMemcpy(dA, hA, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(dB, hB, bytes, cudaMemcpyHostToDevice);

    dim3 threads(BLOCK_SIZE, BLOCK_SIZE);
    dim3 blocks((N + BLOCK_SIZE - 1) / BLOCK_SIZE,
                (N + BLOCK_SIZE - 1) / BLOCK_SIZE);

    // Timing 
    cudaEvent_t start, stop;
    cudaEventCreate(&start); cudaEventCreate(&stop);

    cudaEventRecord(start);
    matmul<<<blocks, threads>>>(dA, dB, dC, N);
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float ms = 0;
    cudaEventElapsedTime(&ms, start, stop);
    cout << "GPU matrix multiplication time: " << ms << " ms\n";

    cudaMemcpy(hC, dC, bytes, cudaMemcpyDeviceToHost);

    // Print corner of result (avoid flooding terminal for large N)
    int preview = min(N, 4);
    cout << "Result (top-left " << preview << "x" << preview << "):\n";
    for (int i = 0; i < preview; i++) {
        for (int j = 0; j < preview; j++)
            cout << hC[i * N + j] << "\t";
        cout << "\n";
    }

    delete[] hA; delete[] hB; delete[] hC;
    cudaFree(dA); cudaFree(dB); cudaFree(dC);
    cudaEventDestroy(start); cudaEventDestroy(stop);
    return 0;
}

// cell 3: !nvcc matrixmul.cu -o matrixmul && ./matrixmul
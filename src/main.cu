#include <iostream>

// Simple CUDA kernel
__global__ void hello_kernel() {
    printf("Hello World from GPU thread %d\n", threadIdx.x);
}

int main() {
    std::cout << "Hello from CPU!" << std::endl;

    // Launch kernel with 5 threads
    hello_kernel<<<1, 5>>>();
    cudaDeviceSynchronize(); // wait for GPU to finish

    return 0;
}


cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(GPUCacheHello LANGUAGES CUDA CXX)

# Set standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_STANDARD 17)

# Add executable from your CUDA file
add_executable(gpu_cache_hello src/main.cu)

# Enable CUDA separate compilation
set_target_properties(gpu_cache_hello PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
)


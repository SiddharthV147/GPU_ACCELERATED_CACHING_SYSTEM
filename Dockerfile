# Use official NVIDIA CUDA image (Ubuntu 22.04 base)
FROM nvidia/cuda:12.6.3-devel-ubuntu22.04

# Prevent interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Update and install tools + GCC 12 + Python
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    gcc-12 g++-12 \
    cmake git wget curl \
    python3 python3-pip python3-venv \
    && rm -rf /var/lib/apt/lists/*

# Set GCC-12 as default
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 100 \
 && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 100

# Working directory
WORKDIR /app

# Copy your source code
COPY . .

# Example: build a CUDA project (change main.cu to your file)
RUN nvcc -std=c++17 -O2 -o main main.cu

# Default command (runs your compiled program)
CMD ["./main"]


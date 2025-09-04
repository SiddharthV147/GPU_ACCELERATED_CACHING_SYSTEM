# Use NVIDIA CUDA Runtime with Ubuntu 22.04 base
FROM nvidia/cuda:12.4.0-runtime-ubuntu22.04

# Avoid interactive prompts during apt installation
ENV DEBIAN_FRONTEND=noninteractive

# Update and install essential packages
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        python3 \
        python3-pip \
        ca-certificates \
        wget \
        git \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Optional: create a non-root user for better security
# RUN useradd -m -u 1000 devuser && chown -R devuser:devuser /workspace
# USER devuser

# Set working directory
WORKDIR /workspace

COPY ./ /workspace/

# Default to bash shell
CMD ["/bin/bash"]


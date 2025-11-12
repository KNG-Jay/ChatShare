# Gather Base Image For Container
FROM ubuntu:latest

# Name Of Author
LABEL authors="kng"

# Working Directory
WORKDIR /app

# Update And Install Tools Needed For Development
RUN apt-get update && apt-get install -y \
    build-essential \
    bash \
    clang \
    cmake \
    ninja-build \
    git \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    bison \
    flex \
    autoconf \
    && rm -rf /var/lib/apt/lists/*

# Install vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git /vcpkg && \
    cd /vcpkg && \
    ./bootstrap-vcpkg.sh

# Environment Variables
ENV VCPKG_ROOT=/vcpkg
EXPOSE 8081

# Copy Contents Of Project Directory Into Image
COPY . .

# Install Library Dependencies Then Build Project
RUN $VCPKG_ROOT/vcpkg integrate install
# RUN $VCPKG_ROOT/vcpkg install
RUN mkdir build && cmake -S . -B build -G Ninja && cmake --build build

# Run Project
CMD ["/bin/sh", "ls"]
#CMD ["./build/runUnitTests"]
#CMD ["./build/ChatShare"]
ENTRYPOINT ["./build/ChatShare"]

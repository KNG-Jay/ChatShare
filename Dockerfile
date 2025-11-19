# Gather Base Image For Container
FROM ubuntu:24.04

# Name Of Author
LABEL authors="kng"

# Update And Install Tools Needed For Development
RUN apt update && apt install -y \
    bash \
    clang \
    cmake \
    ninja-build \
    git \
    build-essential \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    bison \
    flex \
    autoconf \
    autoconf-archive \
    automake \
    #libstdc++-14-dev \
    libc++-dev \
    libc++abi-dev \
    libtool \
    '^libxcb.*-dev' \
    libx11-xcb-dev \
    libglu1-mesa-dev \
    libxrender-dev \
    libxi-dev \
    libxkbcommon-dev \
    libxkbcommon-x11-dev \
    libegl1-mesa-dev \
    libpq-dev \
    && rm -rf /var/lib/apt/lists/*

# Install vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git /vcpkg && \
    cd /vcpkg && \
    ./bootstrap-vcpkg.sh

#RUN echo "export CC=$(which clang)" && echo "export CXX=$(which clang++)"

# Environment Variables
ENV CC=clang CXX=clang++
ENV VCPKG_ROOT=/vcpkg
EXPOSE 8081

# Working Directory
WORKDIR /app

# Copy Contents Of Project Directory Into Image
COPY . .

# Install Library Dependencies Then Build Project
RUN $VCPKG_ROOT/vcpkg integrate install
RUN $VCPKG_ROOT/vcpkg install
RUN mkdir build
RUN cmake -S . -B build -G Ninja
RUN cmake --build build

# Run Project
CMD ["./build/runUnitTests"]
ENTRYPOINT ["./build/ChatShare"]

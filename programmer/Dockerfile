# docker build --progress plain -t pipico:0.0.2 .

# from within the programmer dir
# docker run -v ${PWD}:/programmer/ --privileged=true -u $(id -u ${USER}):$(id -g ${USER}) -it dinoboards/pipico:0.0.2
#
# or
#
# docker run -v ${PWD}:/programmer/ -u $(id -u ${USER}):$(id -g ${USER}) --privileged=true -t dinoboards/pipico:0.0.2 /bin/bash -c "mkdir -p build && cd build && cmake .. && make"

FROM ubuntu:focal-20240530

ENV DEBIAN_FRONTEND=noninteractive

RUN sed -i 's/http:\/\/archive\.ubuntu\.com\/ubuntu/http:\/\/mirror.internode.on.net\/pub\/ubuntu\/ubuntu/g'  /etc/apt/sources.list
RUN sed -i 's/http:\/\/security\.ubuntu\.com\/ubuntu/http:\/\/mirror.internode.on.net\/pub\/ubuntu\/ubuntu/g'  /etc/apt/sources.list

RUN rm -rfv /var/lib/apt/lists/* && \
    apt clean && \
    apt update && \
    apt dist-upgrade -y && \
    apt install -y \
    build-essential \
    curl \
    git \
    python3 \
    software-properties-common \
    wget

LABEL Maintainer="Dean Netherton" \
      Description="complete tool chain to compile c/c++ for pi pico using cmake"

ENV PICO_SDK_PATH=/opt/pico-sdk
ENV PICOTOOL_FETCH_FROM_GIT_PATH=/opt/picotool
WORKDIR /opt/pico-sdk

RUN

RUN adduser --disabled-password --gecos "" builder && \
    \
    apt install -y \
    dirmngr \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    libusb-dev \
    pkg-config && \
    \
    wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null && \
    apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main" && \
    apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 6AF7F09730B3F0A4 && \
    apt update && \
    apt install cmake ninja-build -y && \
    \
    git clone https://github.com/raspberrypi/pico-sdk /opt/pico-sdk &&  \
    cd /opt/pico-sdk && \
    git submodule update --init && \
    mkdir -p build && \
    cd build && \
    cmake .. && \
    make && \
    \
    chown -R builder:builder /opt/pico-sdk && \
    chown -R builder:builder /opt/picotool && \
    \
    mkdir /programmer

WORKDIR /programmer/

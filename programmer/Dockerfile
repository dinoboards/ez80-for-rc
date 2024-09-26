# docker build --progress plain -t pipico .
# docker run -v ${PWD}:/src/ --privileged=true -u $(id -u ${USER}):$(id -g ${USER}) -it pipico

FROM ubuntu:focal-20240530

LABEL Maintainer="Dean Netherton" \
      Description="image of pico sdk tools to build pico pi via cmake"

ENV DEBIAN_FRONTEND=noninteractive

RUN sed -i 's/http:\/\/archive\.ubuntu\.com\/ubuntu/http:\/\/mirror.internode.on.net\/pub\/ubuntu\/ubuntu/g'  /etc/apt/sources.list
RUN sed -i 's/http:\/\/security\.ubuntu\.com\/ubuntu/http:\/\/mirror.internode.on.net\/pub\/ubuntu\/ubuntu/g'  /etc/apt/sources.list

RUN rm -rfv /var/lib/apt/lists/* && apt clean && apt update && apt dist-upgrade -y

RUN apt install -y build-essential wget curl git python3 pkg-config gcc-arm-none-eabi libnewlib-arm-none-eabi software-properties-common

RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null && \
    apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main" && \
    apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 6AF7F09730B3F0A4 && \
    apt update && \
    apt install cmake ninja-build -y

RUN git clone https://github.com/raspberrypi/pico-sdk /opt/pico-sdk &&  \
    cd /opt/pico-sdk && \
    git submodule update --init

ENV PICO_SDK_PATH=/opt/pico-sdk

RUN mkdir /src
WORKDIR /src/

RUN adduser --disabled-password --gecos "" builder

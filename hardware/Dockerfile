# Requires Wincupl files to at ./Wincupl
# Requires ez80-for-rc files to be at ./ez80-for-rc
#
# docker build -f ez80-for-rc/hardware/Dockerfile --progress plain -t dinoboards/wincupl:0.0.1 .
#
# from within the hardware dir
# docker run -v ${PWD}:/ez80-for-rc/hardware/ --privileged=true -u $(id -u ${USER}):$(id -g ${USER}) -it dinoboards/wincupl:0.0.1 ./make-jed.sh

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
      Description="Tool chain to compile JED files from PLD using Wincupl"


RUN dpkg --add-architecture i386

RUN wget -qO- https://dl.winehq.org/wine-builds/Release.key | apt-key add -
RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv F987672F
RUN apt-add-repository 'deb https://dl.winehq.org/wine-builds/ubuntu/ focal main'
RUN apt update -y
RUN apt install -y --no-install-recommends wine32 build-essential

RUN ln -s /usr/lib/wine/wine /usr/bin/wine

COPY --link Wincupl/Shared /Wincupl/Shared

WORKDIR /ez80-for-rc/hardware/

RUN git config --global --add safe.directory /src && git config --global --add safe.directory ./

RUN adduser --disabled-password --gecos "" builder

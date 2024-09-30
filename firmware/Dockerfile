# Requires ZDS files to at ./ZDS
# Requires ez80-for-rc files to be at ./ez80-for-rc

# docker build -f ez80-for-rc/firmware/Dockerfile --progress plain -t ez80-rc-builder .

# from within firmware dir:
# docker run -v ${PWD}:/ez80-for-rc/firmware --privileged=true -u $(id -u ${USER}):$(id -g ${USER}) -it dinoboards/ez80-rc-builder:0.0.1 wine cmd.exe /c "make-debug.bat"

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
      Description="Tool chain for compiling C/C++ with the Zilog ZDS tool set"

RUN apt install -y \
    bison \
    ca-certificates \
    cmake \
    dos2unix \
    dosfstools \
    flex \
    git \
    gnupg2 \
    libboost-all-dev \
    libxml2-dev \
    lzip \
    m4 \
    mtools \
    subversion \
    texi2html \
    texinfo \
    zlib1g-dev && \
    dpkg --add-architecture i386 && \
    wget -O - https://dl.winehq.org/wine-builds/winehq.key | apt-key add - && \
    add-apt-repository 'deb https://dl.winehq.org/wine-builds/ubuntu/ focal main' && \
    apt update && apt install -y --install-recommends winehq-stable

COPY ZDS /ZDS

RUN adduser --disabled-password --gecos "" builder

WORKDIR /ez80-for-rc/firmware

ENV WINEPATH="Z:\\ZDS\\bin\\"

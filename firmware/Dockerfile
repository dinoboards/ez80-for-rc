# Requires ZDS files to at ./ZDS
# Requires ez80-for-rc files to be at ./ez80-for-rc

# docker build -f ez80-for-rc/firmware/Dockerfile --progress plain -t dinoboards/ez80-rc-builder:0.0.2 .

# from within firmware dir:
# docker run --rm -v ${PWD}:/ez80-for-rc/firmware --privileged=true -u $(id -u ${USER}):$(id -g ${USER}) -it dinoboards/ez80-rc-builder:0.0.2 wine cmd.exe /c "make-debug.bat"

FROM ubuntu:noble-20250529

LABEL Maintainer="Dean Netherton" \
      Description="Tool chain for compiling C/C++ with the Zilog ZDS tool set"

ENV DEBIAN_FRONTEND=noninteractive

RUN apt clean && \
    apt update && \
    apt install -y wget gnupg2 software-properties-common && \
    dpkg --add-architecture i386 && \
    mkdir -pm755 /etc/apt/keyrings && \
    wget -O - https://dl.winehq.org/wine-builds/winehq.key | gpg --dearmor -o /etc/apt/keyrings/winehq-archive.key - && \
    wget -NP /etc/apt/sources.list.d/ https://dl.winehq.org/wine-builds/ubuntu/dists/noble/winehq-noble.sources && \
    apt update && \
    apt install -y --no-install-recommends winehq-stable:i386 && \
    apt remove -y wget gnupg2 software-properties-common && \
    apt autoremove -y

COPY ZDS /root/.wine/drive_c/ZDS

RUN adduser --disabled-password --gecos "" builder

WORKDIR /ez80-for-rc/firmware

ENV WINEPATH="C:\\ZDS\\bin\\"

ENV WINEDEBUG=-all

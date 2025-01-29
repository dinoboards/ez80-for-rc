# Requires Wincupl files to at ./Wincupl
# Requires ez80-for-rc files to be at ./ez80-for-rc
#
# run this command in the parent directory of ez80-for-rc
# with Wincupl files prepared - with the fitters updated
# docker build -f ez80-for-rc/hardware/Dockerfile --progress plain -t dinoboards/wincupl:0.0.3 .
#
# from within the ez80-for-rc/hardware directory
# docker run -v ${PWD}:/ez80-for-rc/hardware --privileged=true -u $(id -u ${USER}):$(id -g ${USER}) -it dinoboards/wincupl:0.0.3 ./make-jed.sh
#
# docker push dinoboards/wincupl:0.0.3
FROM ubuntu:jammy-20240911.1
LABEL Maintainer="Dean Netherton" \
      Description="Tool chain to compile JED files from PLD using Wincupl"

ENV DEBIAN_FRONTEND=noninteractive

RUN sed -i 's/http:\/\/archive\.ubuntu\.com\/ubuntu/http:\/\/mirror.internet.asn.au\/pub\/ubuntu\/archive/g'  /etc/apt/sources.list

RUN apt update && \
    apt dist-upgrade -y && \
    apt install -y \
    build-essential \
    curl \
    git \
    python3 \
    software-properties-common \
    wget \
    gpg


RUN dpkg --add-architecture i386
RUN mkdir -pm755 /etc/apt/keyrings
RUN wget -O - https://dl.winehq.org/wine-builds/winehq.key | gpg --dearmor -o /etc/apt/keyrings/winehq-archive.key -
RUN wget -NP /etc/apt/sources.list.d/ https://dl.winehq.org/wine-builds/ubuntu/dists/jammy/winehq-jammy.sources
RUN apt update
RUN apt install -y --install-recommends winehq-stable

COPY --link Wincupl /Wincupl

WORKDIR /ez80-for-rc/hardware/

RUN git config --global --add safe.directory /src && git config --global --add safe.directory ./

RUN adduser --disabled-password --gecos "" builder
RUN chown -R builder:builder /Wincupl

USER builder
ENV XDG_RUNTIME_DIR="/home/builder"

ENV WINEPATH="z:\\Wincupl\\WINCUPL\\EXE\\;z:\\Wincupl\\Shared\\;z:\\WinCupl\\WINCUPL\\FITTERS\\"
ENV FITTERDIR="z:\\WinCupl\\WINCUPL\\FITTERS"
ENV WINEDEBUG="-all"
ENV WINCUPLPATH="z:\\Wincupl\\Shared\\"

ENV WINEARCH=win32
ENV WINEPREFIX=/home/builder/.wine
RUN wine wineboot

RUN wine wineboot

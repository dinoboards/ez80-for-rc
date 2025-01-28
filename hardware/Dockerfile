# Requires Wincupl files to at ./Wincupl
# Requires ez80-for-rc files to be at ./ez80-for-rc
#
# run this command in the parent directory of ez80-for-rc
# with Wincupl files prepeared - with the fitters updated
# docker build -f ez80-for-rc/hardware/Dockerfile --progress plain -t dinoboards/wincupl:0.0.2 .
#
# from within the ez80-for-rc directory
# docker run -v ${PWD}:/ez80-for-rc/hardware/ --privileged=true -u $(id -u ${USER}):$(id -g ${USER}) -it dinoboards/wincupl:0.0.2 ./make-jed.sh

FROM ubuntu:jammy-20240911.1

ENV DEBIAN_FRONTEND=noninteractive


# RUN rm -rfv /var/lib/apt/lists/* && \
#     apt clean && \
#     apt update && \
#     apt dist-upgrade -y && \
#     apt install -y \
#     build-essential \
#     curl \
#     git \
#     python3 \
#     software-properties-common \

LABEL Maintainer="Dean Netherton" \
      Description="Tool chain to compile JED files from PLD using Wincupl"

RUN apt update
RUN apt install -y wget gpg

RUN dpkg --add-architecture i386
RUN mkdir -pm755 /etc/apt/keyrings
RUN wget -O - https://dl.winehq.org/wine-builds/winehq.key | gpg --dearmor -o /etc/apt/keyrings/winehq-archive.key -
RUN wget -NP /etc/apt/sources.list.d/ https://dl.winehq.org/wine-builds/ubuntu/dists/jammy/winehq-jammy.sources
RUN apt update
RUN apt install -y --install-recommends winehq-stable

# RUN wget -qO- https://dl.winehq.org/wine-builds/Release.key | apt-key add -
# RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv F987672F
# RUN apt-add-repository 'deb https://dl.winehq.org/wine-builds/ubuntu/ focal main'
# RUN apt update -y
# RUN apt install -y --no-install-recommends wine32 build-essential

RUN apt-get install -y winetricks playonlinux innoextract
RUN WINEARCH=win32 WINEPREFIX=~/.wine wine wineboot
# RUN winetricks mfc40 mfc42

RUN sed -i 's/http:\/\/archive\.ubuntu\.com\/ubuntu/https:\/\/mirror.internet.asn.au\/pub\/ubuntu\/archive/g'  /etc/apt/sources.list
RUN apt update
RUN apt install -y git

# COPY Wincupl/awincupl.exe .
# RUN wine /tmp/awincupl.exe

# RUN ln -s /usr/lib/wine/wine /usr/bin/wine

COPY --link Wincupl /Wincupl

WORKDIR /ez80-for-rc/hardware/

RUN git config --global --add safe.directory /src && git config --global --add safe.directory ./

RUN adduser --disabled-password --gecos "" builder
RUN chown -R builder:builder /Wincupl

COPY ez80-for-rc/hardware/5vcomp .

ENV WINEPATH="z:\\Wincupl\\WINCUPL\\EXE;z:\\WinCupl\\WINCUPL\\FITTERS"
ENV FITTERDIR="z:\\WinCupl\\WINCUPL\\FITTERS"
ENV WINEDEBUG="fixme-hid,fixme-ntdll,fixme-imm"

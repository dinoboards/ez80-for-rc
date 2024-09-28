FROM ubuntu:jammy-20240111 AS basebuilder

# Requires ZDS files to at ./ZDS
# Requires ez800-for-rc files to be at ./ez80-for-rc

# cd .. && docker build --progress plain -t ez80-rc-builder .

# docker run -v ${PWD}:/ez80-for-rc/ --privileged=true -u $(id -u ${USER}):$(id -g ${USER}) -it ez80-rc-builder wine cmd.exe /c "cd firmware & make-debug.bat"

LABEL Maintainer="Dean Netherton" \
      Description="spike to use zds tools under wine"

ENV DEBIAN_FRONTEND=noninteractive

RUN dpkg --add-architecture i386
RUN sed -i 's/http:\/\/archive\.ubuntu\.com\/ubuntu/http:\/\/mirror.internode.on.net\/pub\/ubuntu\/ubuntu/g'  /etc/apt/sources.list
RUN sed -i 's/http:\/\/security\.ubuntu\.com\/ubuntu/http:\/\/mirror.internode.on.net\/pub\/ubuntu\/ubuntu/g'  /etc/apt/sources.list
RUN rm -rfv /var/lib/apt/lists/* && apt clean && apt update && apt dist-upgrade -y
RUN apt install -y --no-install-recommends cmake lzip ca-certificates mtools build-essential dos2unix libboost-all-dev texinfo texi2html \
                                           libxml2-dev subversion bison flex zlib1g-dev m4 git wget dosfstools curl gnupg2 software-properties-common

RUN wget -O - https://dl.winehq.org/wine-builds/winehq.key | apt-key add - && \
    add-apt-repository 'deb https://dl.winehq.org/wine-builds/ubuntu/ jammy main' && \
    apt update && apt install -y --install-recommends winehq-stable

COPY ZDS /ZDS

RUN adduser --disabled-password --gecos "" builder

WORKDIR /ez80-for-rc

ENV WINEPATH="Z:\\ZDS\\bin\\"

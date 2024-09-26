#!/bin/bash

mkdir -p build
docker run -v ${PWD}:/src/ --privileged=true -u $(id -u ${USER}):$(id -g ${USER}) -it pipico /bin/bash -c "cd /src/build && cmake .. && make -j"

#!/bin/bash

mkdir -p build
docker run --rm -v ${PWD}:/programmer/ --privileged=true -u $(id -u ${USER}):$(id -g ${USER}) -it dinoboards/pipico:0.0.2 /bin/bash -c "cd build && cmake .. && make -j"

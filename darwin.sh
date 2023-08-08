#!/bin/bash

cmake -DCMAKE_BUILD_TYPE=Release -S . -B cmake-build-release
cmake --build cmake-build-release \
--config release \
--target \
client \
server
#write_test \
#fral_ext \
#pc_test -j 4
mv cmake-build-release/fral/python/*.so fral/python/fral

#!/bin/bash

cmake -DCMAKE_BUILD_TYPE=Release -S . -B cmake-build-release -DCMAKE_VERBOSE_MAKEFILE=OFF -DFRAL_PY_VERSION=3.14
cmake --build cmake-build-release \
--config release \
--target \
write_test \
fral_ext \
pc_test -j 4
mv cmake-build-release/fral/python/*.so fral/python/fral

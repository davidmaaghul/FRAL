#!/bin/bash

GRPC_DIR=grpc
INSTALL_DIR=$HOME/.local
BUILD_DIR=cmake/build

git clone --recurse-submodules -b v1.56.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc

cd "${GRPC_DIR}" || exit 1
mkdir -p "${BUILD_DIR}"
pushd "${BUILD_DIR}" || exit 1
cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
      ../.. || exit 1
make -j 4 || exit 1
make install || exit 1
popd || exit 1
cd ..

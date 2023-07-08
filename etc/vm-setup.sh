sudo apt update
sudo apt-get install wget

export MY_INSTALL_DIR=$HOME/.local
mkdir -p $MY_INSTALL_DIR
export PATH="$MY_INSTALL_DIR/bin:$PATH"

sudo apt install -y build-essential autoconf libtool pkg-config

git clone --recurse-submodules -b v1.53.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc
git clone git@github.com:catchorg/Catch2.git

sudo apt-get install libboost-all-dev

cd grpc || exit
mkdir -p cmake/build
pushd cmake/build || exit
cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR ../..
make -j 4
make install
popd || exit
cd ..

cd Catch2 || exit
cmake -Bbuild -H. -DBUILD_TESTING=OFF
sudo cmake --build build/ --target install
cd ..

git clone https://github.com/gflags/gflags.git
cd gflags || exit
mkdir -p build && cd build || exit
cmake ..
make
sudo make install

sudo apt-get -y install python3-pip
sudo apt-get -y install lsof

pip install trio #requirements.txt if we have more later
pip install matplotlib
pip install pandas

git submodule add https://github.com/wjakob/nanobind fral/python/ext/nanobind
git submodule update --init --recursive
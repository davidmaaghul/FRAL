cmake_minimum_required(VERSION 3.18...3.22)
project(FRAL C CXX)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.57.0 COMPONENTS system filesystem program_options regex timer chrono REQUIRED)
find_package(gflags 2.2.2 REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
add_compile_options(-Wall -Wextra -Wpedantic -fPIC)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(fral)

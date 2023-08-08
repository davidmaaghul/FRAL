cmake_minimum_required(VERSION 3.18...3.22)

project(FRAL C CXX)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

if( FRAL_PY_VERSION )
    message("FRAL_PY_VERSION=${FRAL_PY_VERSION}")
else()
    message( FATAL_ERROR "No Python version specified!")
endif()

find_package(Python 3.11.4 COMPONENTS Interpreter Development.Module REQUIRED)
add_subdirectory(ext/nanobind)
find_package(Boost 1.57.0 COMPONENTS system filesystem program_options regex timer chrono REQUIRED)
find_package(gflags 2.2.2 REQUIRED)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
add_compile_options(-Wall -Wextra -Wpedantic -fPIC -O3 -DSQLITE_THREADSAFE=1)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(fral)

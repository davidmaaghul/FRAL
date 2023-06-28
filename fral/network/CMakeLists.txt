set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

get_filename_component(medium "medium.proto" ABSOLUTE)
get_filename_component(medium_path "${medium}" PATH)

# Generated sources
set(medium_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/medium.pb.cc")
set(medium_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/medium.pb.h")
set(medium_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/medium.grpc.pb.cc")
set(medium_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/medium.grpc.pb.h")


add_custom_command(
        OUTPUT "${medium_proto_srcs}" "${medium_proto_hdrs}" "${medium_grpc_srcs}" "${medium_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out ${CMAKE_CURRENT_BINARY_DIR}
        --cpp_out ${CMAKE_CURRENT_BINARY_DIR}
        -I "${medium_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${medium}"
        DEPENDS "${medium}")
#
## Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
#
add_library(medium_grpc_proto
        ${medium_grpc_srcs}
        ${medium_grpc_hdrs}
        ${medium_proto_srcs}
        ${medium_proto_hdrs})

target_link_libraries(medium_grpc_proto
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})

add_library(network server.cpp client.cpp)

target_link_libraries(network
        medium_grpc_proto
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF}
        fral_engine
        )

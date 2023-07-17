add_executable(write_test write_test.cpp)
add_executable(pc_test pc_test.cpp utility.h)
add_executable(client client.cpp utility.h)
add_executable(server server.cpp utility.h)

target_link_libraries(write_test fral_engine gflags)
target_link_libraries(pc_test fral_engine gflags)
target_link_libraries(client fral_engine gflags network)
target_link_libraries(server fral_engine gflags network)

add_executable(write_test write_test.cpp)
add_executable(pc_test pc_test.cpp utility.h)
add_executable(client client.cpp utility.h)
add_executable(server server.cpp utility.h)
add_executable(write_test_sqlite write_test_sqlite.cpp engine_sqlite.h)
add_executable(pc_test_sqlite pc_test_sqlite.cpp engine_sqlite.h)

target_link_libraries(write_test fral_engine gflags)
target_link_libraries(pc_test fral_engine gflags)
target_link_libraries(client fral_engine gflags network)
target_link_libraries(server fral_engine gflags network)
target_link_libraries(write_test_sqlite gflags sqlite3)
target_link_libraries(pc_test_sqlite gflags sqlite3)
find_package(GTest REQUIRED)
add_executable(cpp_unit_tests test_engine.cpp)
target_link_libraries(cpp_unit_tests GTest::GTest fral_engine)

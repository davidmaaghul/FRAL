find_package(Python 3.11.4 COMPONENTS Interpreter Development.Module REQUIRED)

add_subdirectory(ext/nanobind)
nanobind_add_module(fral_ext fral_ext.cpp)
target_link_libraries(fral_ext PRIVATE fral_engine)
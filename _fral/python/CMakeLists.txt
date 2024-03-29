nanobind_add_module(fral_ext STABLE_ABI fral/fral_ext.cpp)
target_link_libraries(fral_ext PRIVATE fral_engine)
install(TARGETS fral_ext LIBRARY DESTINATION fral)
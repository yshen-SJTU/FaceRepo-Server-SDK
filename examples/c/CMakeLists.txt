cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(example)

link_libraries("/usr/lib64/libxcaan_faceengine.so")
link_libraries("/usr/lib64/libxcaan_facerepo.so")

add_executable(main main.cpp)

target_link_libraries(main "xcaan_faceengine" "xcaan_facerepo")

set_property(TARGET main PROPERTY CXX_STANDARD 14)

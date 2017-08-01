cmake_minimum_required(VERSION 3.7)
project(sketch)

set(CMAKE_CXX_STANDARD 11)
include(FindPCAP.cmake)

set(SOURCE_FILES
        src/bucket.cpp src/bucket.h
        src/sketch.cpp src/sketch.h
        lib/easylogging++.cc lib/easylogging++.h
        lib/catch.hpp
        test/tests-main.cpp
        test/tests-sketch.cpp
        test/tests-bucket.cpp
        lib/pcap_parser.hpp
        lib/key_generator.hpp lib/typedefs.h lib/utils.hpp)

add_executable(sketch ${SOURCE_FILES})
target_link_libraries(sketch ${PCAP_LIBRARY})

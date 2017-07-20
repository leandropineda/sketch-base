cmake_minimum_required(VERSION 3.7)
project(sketch)

set(CMAKE_CXX_STANDARD 11)
include(FindPCAP.cmake)

set(SOURCE_FILES
        src/key.cpp src/key.h
        src/bucket.cpp src/bucket.h
        src/sketch.cpp src/sketch.h
        lib/easylogging++.cc lib/easylogging++.h
        lib/catch.hpp
        lib/utils.hpp
        test/tests-main.cpp
        test/tests-key.cpp
        test/tests-sketch.cpp
        test/tests-bucket.cpp
        test/tests-key-generator.cpp
        lib/pcap_parser.hpp
        lib/key_generator.hpp lib/typedefs.h)

add_executable(sketch ${SOURCE_FILES})
target_link_libraries(sketch ${PCAP_LIBRARY})

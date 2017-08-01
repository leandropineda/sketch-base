cmake_minimum_required(VERSION 3.7)
project(sketch)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/bucket.cpp src/bucket.h
        src/sketch.cpp src/sketch.h
        lib/easylogging++.cc lib/easylogging++.h
        lib/catch.hpp
        test/tests-main.cpp
        test/tests-sketch.cpp
        test/tests-bucket.cpp
        lib/typedefs.h lib/utils.hpp)

add_executable(sketch ${SOURCE_FILES})

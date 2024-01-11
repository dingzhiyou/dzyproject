cmake_minimum_required(VERSION 2.8)
project(dzy)


set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_COMPILER "g++")
set(CMKAE_CXX_FLAGS "$ENV(CXXFLAGS -rdynamic) -O3 -ggdb std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

set (LIB_SRC
    dzy/log.cc
    dzy/config.cc
    dzy/util.cc
    )

add_library(dzy SHARED ${LIB_SRC})
include_directories(./yaml-cpp/include/yaml-cpp)
include_directories(./dzy)


add_executable(test tests/test.cc)
add_dependencies(test dzy)
target_link_libraries(test dzy yaml-cpp)


add_executable(test_config tests/test_config.cc)
add_dependencies(test_config dzy yaml-cpp)
target_link_libraries(test_config dzy yaml-cpp)





set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)




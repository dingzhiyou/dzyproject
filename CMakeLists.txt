cmake_minimum_required(VERSION 2.8)
project(dzy)


set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")
set (LIB_SRC
    dzy/log.cc
    dzy/config.cc
    dzy/util.cc
    dzy/thread.cc
    dzy/fiber.cc
    )

set(LIB_LIB 
    dzy
    pthread
    yaml-cpp
    )

add_library(dzy SHARED ${LIB_SRC})
include_directories(./yaml-cpp/include/yaml-cpp)
include_directories(./dzy)
include_directories(./tests)


add_executable(test tests/test.cc)
add_dependencies(test dzy)
target_link_libraries(test ${LIB_LIB})


add_executable(test_backtrace tests/test_backtrace.cc)
add_dependencies(test_backtrace dzy)
target_link_libraries(test_backtrace ${LIB_LIB})

add_executable(test_fiber tests/test_fiber.cc)
add_dependencies(test_fiber dzy)
target_link_libraries(test_fiber ${LIB_LIB})




add_executable(test_config tests/test_config.cc)
add_dependencies(test_config dzy yaml-cpp)
target_link_libraries(test_config ${LIB_LIB})


add_executable(test_config_log tests/test_config_log.cc)
add_dependencies(test_config_log dzy)
target_link_libraries(test_config_log ${LIB_LIB})


add_executable(test_thread tests/test_thread.cc)
add_dependencies(test_thread dzy)
target_link_libraries(test_thread ${LIB_LIB})

add_executable(test_mutex tests/test_mutex.cc)
add_dependencies(test_mutex dzy)
target_link_libraries(test_mutex ${LIB_LIB})



set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)




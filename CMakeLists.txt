cmake_minimum_required(VERSION 2.8)
project(dzy)


set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMKAE_CXX_FLAGS "$ENV(CXXFLAGS -rdynamic) -O3 -ggdb std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

set (LIB_SRC
    dzy/log.cc
    )

add_library(dzy SHARED ${LIB_SRC})



add_executable(test tests/test.cc)
add_dependencies(test dzy)
target_link_libraries(test dzy)

add_executable(test_shared_ptr tests/test_shared_ptr.cc)
add_dependencies(test_shared_ptr dzy)
target_link_libraries(test_shared_ptr dzy)





set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)




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
    dzy/fd_manager.cc
    dzy/schedule.cc
    dzy/iomanager.cc
    dzy/timer.cc
    dzy/hook.cc
    dzy/addr.cc
    dzy/socket.cc
    dzy/sockstream.cc
    dzy/bytearray.cc
    dzy/http/http.cc
    dzy/http/http11_parser.rl.cc
    dzy/http/httpclient_parser.rl.cc
    dzy/http/http_parser.cc
    dzy/http/http_session.cc
    dzy/http/http_server.cc
    dzy/http/servlet.cc
    dzy/ftp/ftp_server.cc
    dzy/tcp_server.cc
    )

set(LIB_LIB 
    dzy
    pthread
    yaml-cpp
    dl
    )

add_library(dzy SHARED ${LIB_SRC})
include_directories(./yaml-cpp/include/yaml-cpp)
include_directories(./dzy)
include_directories(./tests)

add_executable(test_sock_client tests/test_sock_client.cc)
add_dependencies(test_sock_client dzy)
target_link_libraries(test_sock_client ${LIB_LIB})


add_executable(test_addr tests/test_addr.cc)
add_dependencies(test_addr dzy)
target_link_libraries(test_addr ${LIB_LIB})


add_executable(test_ftp_server tests/test_ftp_server.cc)
add_dependencies(test_ftp_server dzy)
target_link_libraries(test_ftp_server ${LIB_LIB})

add_executable(test_tcp_server tests/test_tcp_server.cc)
add_dependencies(test_tcp_server dzy)
target_link_libraries(test_tcp_server ${LIB_LIB})

add_executable(test tests/test.cc)
add_dependencies(test dzy)
target_link_libraries(test ${LIB_LIB})

add_executable(test_hook tests/test_hook.cc)
add_dependencies(test_hook dzy)
target_link_libraries(test_hook ${LIB_LIB})

add_executable(test_backtrace tests/test_backtrace.cc)
add_dependencies(test_backtrace dzy)
target_link_libraries(test_backtrace ${LIB_LIB})

add_executable(test_fiber tests/test_fiber.cc)
add_dependencies(test_fiber dzy)
target_link_libraries(test_fiber ${LIB_LIB})


add_executable(test_iomanager tests/test_iomanager.cc)
add_dependencies(test_iomanager dzy)
target_link_libraries(test_iomanager ${LIB_LIB})


add_executable(test_bytearray tests/test_bytearray.cc)
add_dependencies(test_bytearray  dzy)
target_link_libraries(test_bytearray  ${LIB_LIB})


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


add_executable(test_schedule tests/test_schedule.cc)
add_dependencies(test_schedule dzy)
target_link_libraries(test_schedule  ${LIB_LIB})


add_executable(test_http tests/test_http.cc)
add_dependencies(test_http dzy)
target_link_libraries(test_http ${LIB_LIB})


add_executable(test_http_server tests/test_http_server.cc)
add_dependencies(test_http_server dzy)
target_link_libraries(test_http_server ${LIB_LIB})

add_executable(echo_server examples/echo_server.cc)
add_dependencies(echo_server dzy)
target_link_libraries(echo_server ${LIB_LIB})

add_executable(test_http_parser tests/test_http_parser.cc)
add_dependencies(test_http_parser dzy)
target_link_libraries(test_http_parser  ${LIB_LIB})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)




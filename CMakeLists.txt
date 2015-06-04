cmake_minimum_required(VERSION 3.1)
project(ComputerArchitecture4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpthread -lboost_serialization -lboost_program_options -lboost_system")

set(SOURCE_FILES server.cpp)
add_executable(ComputerArchitecture4 ${SOURCE_FILES} thread/myThread.h thread/myThread.cpp socket/TCPSocket.cpp socket/TCPSocket.h connection/connection.h tcp_server/asioTcpServer.h)

cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME AsioServer)
project(${PROJECT_NAME} C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

##########################################################
# Preprocessor definitions
##########################################################

add_definitions(-DASIO_STANDALONE)

##########################################################
# Project files, packages, libraries and so on
##########################################################

file(GLOB SOURCES source/*.cpp)

add_executable(${PROJECT_NAME} source/server.cpp ${NETWORK_SOURCES})

########
# Asio #
########
include_directories(${PROJECT_NAME} vendor/asio/asio/include)

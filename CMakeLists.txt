cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME async_server)
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

#file(GLOB SOURCES source/*.cpp)

add_executable(asio_server source/server_entry_point.cpp)
add_executable(asio_client source/client_entry_point.cpp)

########
# Asio #
########
include_directories(asio_server vendor/asio/asio/include)
include_directories(asio_client vendor/asio/asio/include)

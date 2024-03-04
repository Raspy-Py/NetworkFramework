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

include_directories(vendor/asio/asio/include)

add_executable(server source/server.cpp)
add_executable(client source/client.cpp)

########
# Asio #
########
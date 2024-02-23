if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

# Warnings as errors should be imported here -- do not move this line
include(cmake/CompilerWarnings.cmake)


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-fsanitize=thread" COMPILER_SUPPORTS_TSAN)
if (COMPILER_SUPPORTS_TSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no Thread Sanitizer support.")
endif()

##########################################################
# Preprocessor definitions
##########################################################

add_definitions(-DASIO_STANDALONE)

##########################################################
# Project files, packages, libraries and so on
##########################################################

file(GLOB SOURCES source/*.cpp)

add_executable(Server source/server.cpp ${NETWORK_SOURCES})

########
# Asio #
########
include_directories(Server vendor/asio/asio/include)

########################
# Tine process library #
########################
add_subdirectory(vendor/tiny-process-library)
#target_link_libraries(${PROJECT_NAME} PRIVATE tiny-process-library)
#include_directories(${PROJECT_NAME} vendor/tiny-process-library)

target_link_libraries(Client PRIVATE tiny-process-library)
include_directories(Client vendor/tiny-process-library)

##########################################################
# Fixed CMakeLists.txt part
##########################################################

INSTALL(PROGRAMS
        #$<TARGET_FILE:${PROJECT_NAME}> # ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
        $<TARGET_FILE:Server>
        $<TARGET_FILE:Client>
        DESTINATION bin)

# Define ALL_TARGETS variable to use in PVS and Sanitizers
#set(ALL_TARGETS ${PROJECT_NAME})
set(ALL_TARGETS Server Client)

# Include CMake setup
include(cmake/main-config.cmake)
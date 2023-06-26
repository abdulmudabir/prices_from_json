cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME "euraud")
project(${PROJECT_NAME} CXX)
message(STATUS "Project Name: ${PROJECT_NAME}")

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "Compilers other than GCC are not supported at this point.")
endif()

if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "8.1")
    message(FATAL_ERROR "Need GCC Version 8.1 and higher.")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILER_FLAGS} -Wall -Werror -Wextra -Wpedantic -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CXX_COMPILER_FLAGS} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CXX_COMPILER_FLAGS} -g -O0 -DDEBUG")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)
configure_file(config/config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/include/config.hpp)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCES
    ${SOURCE_DIR}/main.cpp
)

set(APP "euraud")
add_executable(${APP} ${SOURCES})

install(TARGETS ${APP} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)

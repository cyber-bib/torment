cmake_minimum_required(VERSION 3.29.0 FATAL_ERROR)

project(torment-prj)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

include(GoogleTest)

find_package(Boost REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

add_subdirectory(src)

enable_testing()

find_package(GTest CONFIG REQUIRED)


add_subdirectory(tests)
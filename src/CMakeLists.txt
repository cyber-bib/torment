cmake_minimum_required(VERSION 3.29.0 FATAL_ERROR)

project(torment)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

find_package(Boost REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

add_executable(tor-mentor
  main.cxx
  # vector.hxx
  # vector.cxx
  # matrix.hxx
  # matrix.cxx
  tormentor.hxx
  tormentor.cxx  )
target_include_directories(tor-mentor
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}  )

# target_link_libraries(main PRIVATE Boost::boost Boost::<lib1> Boost::<lib2> ...)
target_link_libraries(tor-mentor
  PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    Boost::boost  )
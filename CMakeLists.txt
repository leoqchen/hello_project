cmake_minimum_required(VERSION 3.4...3.20 FATAL_ERROR)
project(hello_project)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(O -O0 -g -Wall)
add_compile_options(-std=c++20 ${O})

add_library(
  aes
  STATIC
  src/aes.cpp
)

add_executable(
  test
  src/test.cpp
)
target_link_libraries(
  test
  aes
)
target_compile_options(
  test
  PUBLIC
  # xxx
)

add_executable(
  test2
  src/test2.cpp
)
target_link_libraries(
  test2
  aes
)
target_compile_options(
  test2
  PUBLIC
  # xxx
)

add_executable(
  test3
  src/test3.cpp
)
target_link_libraries(
  test3
  aes
)
target_compile_options(
  test3
  PUBLIC
  # xxx
)

add_executable(
  test5_MyFile
  src/test5_MyFile.cpp
)
target_link_libraries(
  test5_MyFile
  aes
)
target_compile_options(
  test5_MyFile
  PUBLIC
  # xxx
)

add_executable(
  test6
  src/test6.c
)
target_link_libraries(
  test6
  # xxx
)
target_compile_options(
  test6
  PUBLIC
  # xxx
)

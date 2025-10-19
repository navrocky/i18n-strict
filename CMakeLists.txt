cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(i18n_strict LANGUAGES CXX)

option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
  add_subdirectory(test)
endif()


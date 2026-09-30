cmake_minimum_required(VERSION 3.16)
project(KMPStringSearch CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(kmp_lib src/kmp.cpp)
target_include_directories(kmp_lib PUBLIC src)

add_executable(kmp src/main.cpp)
target_link_libraries(kmp PRIVATE kmp_lib)

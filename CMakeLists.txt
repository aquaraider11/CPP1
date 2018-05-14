cmake_minimum_required(VERSION 3.10)
project(CPP1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")

include_directories(.)


add_executable(CPP1
        main.cpp)

target_link_libraries(CPP1 curl pthread stdc++fs sfml-graphics sfml-window sfml-system)
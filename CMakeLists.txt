cmake_minimum_required(VERSION 3.21)
project(easywm)

set(CMAKE_CXX_STANDARD 23)

find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES})

include_directories(${X11_INCLUDE_DIR})

add_executable(easywm main.cpp)

target_include_directories(easywm PRIVATE ./lib)

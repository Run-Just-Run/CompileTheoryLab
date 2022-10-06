cmake_minimum_required(VERSION 3.21)
project(Scanner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

ADD_SUBDIRECTORY(vendor/spdlog-1.x)

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

add_executable(Scanner src/main.cpp src/Scanner.cpp src/Scanner.h src/Core.h src/Log.cpp src/Log.h)
target_link_libraries(Scanner PRIVATE spdlog::spdlog)

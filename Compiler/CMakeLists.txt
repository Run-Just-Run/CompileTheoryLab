cmake_minimum_required(VERSION 3.21)
project(Compiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

ADD_SUBDIRECTORY(vendor/spdlog-1.x)

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

add_executable(Compiler main.cpp Timer.h Scanner/src/Scanner.cpp Scanner/src/Scanner.h Log.cpp Log.h Core.h Parser/src/RecurParser.cpp Parser/src/RecurParser.h Parser/src/ASTNode.h Parser/src/LRParser.cpp Parser/src/LRParser.h Parser/src/Parser.h Parser/src/Parser.cpp)
target_link_libraries(Compiler PRIVATE spdlog::spdlog)

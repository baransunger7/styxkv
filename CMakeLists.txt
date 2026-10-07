cmake_minimum_required(VERSION 4.3)
project(styxKV)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(styxKV
        src/main.cpp
        include/styx/common/types.hpp
        include/styx/storage/hash_table.hpp
        include/styx/engine/command_dispatcher.hpp
        src/storage/hash_table.cpp
        src/engine/command_dispatcher.cpp
        include/styx/engine/command.hpp
        include/styx/utils/overloaded.hpp)
target_include_directories(styxKV PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

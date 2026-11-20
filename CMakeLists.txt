cmake_minimum_required(VERSION 3.25)
project(styxKV)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

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
        include/styx/utils/overloaded.hpp
        include/styx/engine/create_command.hpp
        src/engine/create_command.cpp
        include/styx/utils/string_helpers.hpp
        include/styx/engine/get_command.hpp
        src/engine/get_command.cpp
        include/styx/engine/set_command.hpp
        src/engine/set_command.cpp
        include/styx/engine/del_command.hpp
        src/engine/del_command.cpp
        include/styx/engine/type_command.hpp
        src/engine/type_command.cpp
        include/styx/engine/exists_command.hpp
        src/engine/exists_command.cpp
        src/utils/string_helpers.cpp
        include/styx/engine/keys_command.hpp
        src/engine/keys_command.cpp
        include/styx/engine/lpush_command.hpp
        src/engine/lpush_command.cpp
        include/styx/engine/rpush_command.hpp
        src/engine/rpush_command.cpp
        include/styx/engine/lpop_command.hpp
        src/engine/lpop_command.cpp
        include/styx/engine/rpop_command.hpp
        src/engine/rpop_command.cpp
        include/styx/engine/sadd_command.hpp
        src/engine/sadd_command.cpp
        include/styx/engine/srem_command.hpp
        src/engine/srem_command.cpp
        include/styx/utils/time_utils.hpp
        include/styx/engine/expire_command.hpp
        src/engine/expire_command.cpp
        include/styx/engine/ttl_command.hpp
        src/engine/ttl_command.cpp
        include/styx/storage/persistence_manager.hpp
        src/storage/persistence_manager.cpp
        include/styx/engine/expireat_command.hpp
        src/engine/expireat_command.cpp)
target_include_directories(styxKV PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

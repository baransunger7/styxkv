#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "styx/storage/hash_table.hpp"
#include "styx/engine/create_command.hpp"
#include "styx/engine/get_command.hpp"
#include "styx/engine/command_dispatcher.hpp"
#include "styx/engine/del_command.hpp"
#include "styx/engine/exists_command.hpp"
#include "styx/engine/expire_command.hpp"
#include "styx/engine/keys_command.hpp"
#include "styx/engine/lpop_command.hpp"
#include "styx/engine/lpush_command.hpp"
#include "styx/engine/rpop_command.hpp"
#include "styx/engine/rpush_command.hpp"
#include "styx/engine/sadd_command.hpp"
#include "styx/engine/set_command.hpp"
#include "styx/engine/srem_command.hpp"
#include "styx/engine/ttl_command.hpp"
#include "styx/engine/type_command.hpp"




int main() {
    styx::storage::HashTable table;
    styx::engine::CommandDispatcher dispatcher;

    dispatcher.registerCommand("create", std::make_unique<styx::engine::CreateCommand>());
    dispatcher.registerCommand("get", std::make_unique<styx::engine::GetCommand>());
    dispatcher.registerCommand("set",std::make_unique<styx::engine::SetCommand>());
    dispatcher.registerCommand("del",std::make_unique<styx::engine::DelCommand>());
    dispatcher.registerCommand("type",std::make_unique<styx::engine::TypeCommand>());
    dispatcher.registerCommand("exists",std::make_unique<styx::engine::ExistsCommand>());
    dispatcher.registerCommand("keys",std::make_unique<styx::engine::KeysCommand>());
    dispatcher.registerCommand("lpush",std::make_unique<styx::engine::LPushCommand>());
    dispatcher.registerCommand("rpush",std::make_unique<styx::engine::RPushCommand>());
    dispatcher.registerCommand("lpop",std::make_unique<styx::engine::LPopCommand>());
    dispatcher.registerCommand("rpop",std::make_unique<styx::engine::RPopCommand>());
    dispatcher.registerCommand("sadd",std::make_unique<styx::engine::SAddCommand>());
    dispatcher.registerCommand("srem",std::make_unique<styx::engine::SRemCommand>());
    dispatcher.registerCommand("expire",std::make_unique<styx::engine::ExpireCommand>());
    dispatcher.registerCommand("ttl",std::make_unique<styx::engine::TTLCommand>());
    std::cout << "StyxKV CLI (type 'exit' to quit)\n";
    std::cout << "--------------------------------\n";

    std::string line;
    while (true) {
        std::cout << "styx> ";
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;
        if (line == "exit" || line == "quit") break;
        auto result = dispatcher.dispatch(line,table);
        std::cout << result.message << std::endl;

    }

    return 0;
}

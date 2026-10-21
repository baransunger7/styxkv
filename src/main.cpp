#include <chrono>
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
#include "styx/engine/expireat_command.hpp"
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
#include "styx/storage/persistence_manager.hpp"
#include  "styx/utils/string_helpers.hpp"

bool isMutatingCommand(std::string cmd) {
    cmd = styx::utils::toLower(cmd);
    return  cmd == "set" || cmd == "create" || cmd == "del" ||
            cmd == "lpush" || cmd == "rpush" || cmd == "lpop" || cmd == "rpop" ||
            cmd == "expire"  || cmd == "expireat" ||
            cmd == "sadd" || cmd == "srem";
}


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
    dispatcher.registerCommand("expireat",std::make_unique<styx::engine::ExpireAtCommand>());
    dispatcher.registerCommand("ttl",std::make_unique<styx::engine::TTLCommand>());
    std::cout << "StyxKV CLI (type 'exit' to quit)\n";
    std::cout << "--------------------------------\n";

    styx::storage::PersistenceManager pm{"styx.aof"};
    pm.recover(table,dispatcher);
    table.setOnKeyDeleted([&pm](const std::string& key) {
    pm.logCommand("del "  + key);
    });
    std::string line;
    while (true) {
        std::cout << "styx> ";
        if (!std::getline(std::cin, line)) break;
        std::vector<std::string> tokens = styx::utils::tokenize(line);
        if (tokens.empty()) continue;
        std::string cmd = styx::utils::toLower(tokens[0]);
        if (cmd == "exit" || cmd == "quit") break;
        auto result = dispatcher.dispatch(line,table);
        if (result.status == styx::common::CommandStatus::Ok && isMutatingCommand(cmd)) {
            if (cmd == "expire") {
                auto now_epoch = std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count();
                auto seconds = std::stoll(tokens[2]);
                uint64_t absolute_timestamp = now_epoch + seconds;
                line = "expireat " + tokens[1] + " " + std::to_string(absolute_timestamp);
            }
            pm.logCommand(line);
        }



        std::cout << result.message << std::endl;

    }

    return 0;
}

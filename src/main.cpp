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
#include "styx/engine/set_command.hpp"
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

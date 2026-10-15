#include "styx/engine/command_dispatcher.hpp"
#include "styx/utils/string_helpers.hpp"
#include <algorithm>
#include <cctype>

namespace styx::engine {
styx::common::CommandResult CommandDispatcher::dispatch(std::string_view rawInput, styx::storage::HashTable &table) {
    std::vector<std::string> tokens;
    try {
        tokens = styx::utils::tokenize(rawInput);
    } catch (const std::exception& e) {
        return styx::common::CommandResult{
            styx::common::CommandStatus::Error,
            "ERR " + std::string(e.what())
        };
    }

    if (tokens.empty()) {
        return styx::common::CommandResult{
            styx::common::CommandStatus::Empty,
            ""
        };
    }
    std::string cmdName = styx::utils::toLower(tokens[0]);
    auto it = commands_.find(cmdName);
    if (it == commands_.end()) {
        return styx::common::CommandResult{styx::common::CommandStatus::Error,"ERR Unknown command"};
    }
    std::vector<std::string> args;
    args.reserve(tokens.size() - 1);
    for (size_t i = 1; i < tokens.size(); ++i) {
        args.push_back(std::move(tokens[i]));
    }
    return it->second->execute(args, table);
}

void CommandDispatcher::registerCommand(std::string name, std::unique_ptr<Command> cmd) {
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    if (commands_.find(name) != commands_.end()) {
        return;
    }

    commands_[std::move(name)] = std::move(cmd);
}
}

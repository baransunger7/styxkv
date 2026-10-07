#include "styx/engine/command_dispatcher.hpp"
#include <algorithm>
#include <cctype>

namespace styx::engine {
styx::common::CommandResult CommandDispatcher::dispatch(std::string_view rawInput, styx::storage::HashTable &table) {
    std::vector<std::string_view> tokens;
    constexpr std::string_view delimiters = " \t\r\n";
    size_t start = rawInput.find_first_not_of(delimiters);
    if (start == std::string_view::npos) {
        return styx::common::CommandResult{styx::common::CommandStatus::Empty,""};
    }
    while (start != std::string_view::npos) {
        size_t end = rawInput.find_first_of(delimiters, start);
        if (end == std::string_view::npos) {
            tokens.push_back(rawInput.substr(start));
            break;
        }
        tokens.push_back(rawInput.substr(start, end - start));
        start = rawInput.find_first_not_of(delimiters, end);
    }
    std::string cmdName(tokens[0]);
    std::transform(cmdName.begin(), cmdName.end(), cmdName.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    auto it = commands_.find(cmdName);
    if (it == commands_.end()) {
        return styx::common::CommandResult{styx::common::CommandStatus::Error,"ERR Unknown command"};
    }
    std::vector<std::string> args;
    args.reserve(tokens.size() - 1);
    for (size_t i = 1; i < tokens.size(); ++i) {
        args.emplace_back(tokens[i]);
    }    return it->second->execute(args, table);
    //devam edilcek
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

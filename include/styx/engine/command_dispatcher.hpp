#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include "styx/engine/command.hpp"


namespace styx::engine {

class CommandDispatcher {
public:
    CommandDispatcher() = default;
    ~CommandDispatcher() = default;

    void registerCommand(std::string name,std::unique_ptr<Command> cmd);

    styx::common::CommandResult dispatch(std::string_view rawInput, styx::storage::HashTable& table);

private:
    std::unordered_map<std::string, std::unique_ptr<Command>> commands_;
};
}

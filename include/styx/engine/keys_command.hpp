#pragma once

#include "styx/engine/command.hpp"


namespace styx::engine {

class KeysCommand : public Command {
public:
    KeysCommand() = default;
    styx::common::CommandResult execute(const std::vector<std::string> &args, styx::storage::HashTable &table) override;
};
}
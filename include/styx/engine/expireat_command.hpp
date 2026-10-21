#pragma once

#include "styx/engine/command.hpp"

namespace styx::engine {

class ExpireAtCommand : public Command {
public:
    ExpireAtCommand() = default;
    styx::common::CommandResult execute(const std::vector<std::string> &args, styx::storage::HashTable &table) override;
};
}
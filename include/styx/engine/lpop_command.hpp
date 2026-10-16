#pragma once

#include "styx/engine/command.hpp"

namespace styx::engine {
class LPopCommand : public Command {
public:
    LPopCommand() = default;
    styx::common::CommandResult execute(const std::vector<std::string> &args, styx::storage::HashTable &table) override;
};
}
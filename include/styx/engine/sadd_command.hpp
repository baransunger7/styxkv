#pragma once

#include "styx/engine/command.hpp"

namespace styx::engine {
class SAddCommand : public Command {
public:
    SAddCommand() = default;
    styx::common::CommandResult execute(const std::vector<std::string> &args, styx::storage::HashTable &table) override;
};
}
#pragma once
#include "command.hpp"

namespace styx::engine {

class TTLCommand : public Command {
public:
    TTLCommand() = default;
    styx::common::CommandResult execute(const std::vector<std::string> &args, styx::storage::HashTable &table) override;
};
}

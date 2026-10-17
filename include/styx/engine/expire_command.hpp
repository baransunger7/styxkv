#pragma once
#include "command.hpp"

namespace styx::engine {

class ExpireCommand : public Command {
public:
    ExpireCommand() = default;
    styx::common::CommandResult execute(const std::vector<std::string> &args, styx::storage::HashTable &table) override;
};
}

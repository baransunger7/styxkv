#pragma once
#include <string>
#include <vector>
#include "styx/common/types.hpp"
#include "styx/storage/hash_table.hpp"

namespace styx::engine {

class Command {
public:
    virtual ~Command() = default;
    virtual styx::common::CommandResult execute(const std::vector<std::string>& args,styx::storage::HashTable& table) = 0;
};
}

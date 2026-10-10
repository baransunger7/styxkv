#pragma once
#include "styx/engine/command.hpp"

namespace styx::engine {

class CreateCommand : public Command {
public:
    CreateCommand() = default;
    styx::common::CommandResult execute(const std::vector<std::string> &args, styx::storage::HashTable &table) override;
private:
    styx::common::CommandResult createList(const std::vector<std::string> &args, styx::storage::HashTable &table);
    styx::common::CommandResult createSet(const std::vector<std::string> &args, styx::storage::HashTable &table);
    styx::common::CommandResult createHash(const std::vector<std::string> &args, styx::storage::HashTable &table);
    styx::common::CommandResult createString(const std::vector<std::string> &args, styx::storage::HashTable &table);

};

}
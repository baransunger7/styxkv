#pragma once
#include <span>

#include "styx/engine/command.hpp"
#include "styx/common/types.hpp"
#include "styx/storage/hash_table.hpp"

namespace styx::engine {

class SetCommand : public Command {
public:
    SetCommand() = default;
    styx::common::CommandResult execute(const std::vector<std::string>& args,styx::storage::HashTable& table) override;
private:
    static void updateList(std::vector<std::string>& list,std::span<const std::string> subArgs);
    static void updateHash(std::unordered_map<std::string,std::string>& hash,std::span<const std::string> subArgs);
    static void updateSet(std::unordered_set<std::string>& set,std::span<const std::string> subArgs);

};
}

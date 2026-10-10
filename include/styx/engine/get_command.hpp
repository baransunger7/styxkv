#pragma once

#include "styx/engine/command.hpp"
#include <vector>
#include <string>
#include <span>

namespace styx::engine {

class GetCommand : public Command {
public:
    GetCommand() = default;
    styx::common::CommandResult execute(const std::vector<std::string> &args, styx::storage::HashTable &table) override;
private:
    std::string formatString(const std::string& value,std::span<const std::string> subArgs) const;
    std::string formatList(const std::vector<std::string>& list,std::span<const std::string> subArgs) const;
    std::string formatHash(const std::unordered_map<std::string,std::string>& hash,std::span<const std::string> subArgs) const;
    std::string formatSet(const std::unordered_set<std::string>& set,std::span<const std::string> subArgs) const;
};
}
#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

namespace styx::common {
using DbList = std::vector<std::string>;
using DbSet = std::unordered_set<std::string>;
using DbHash = std::unordered_map<std::string, std::string>;
using DbString = std::string;

using DbValue = std::variant<DbList, DbSet, DbHash, DbString>;

struct Entry {
    DbValue value;
    uint64_t expires_at{0};
    uint8_t flags{0};
};

enum class CommandStatus {
    Ok,
    Error,
    NotFound,
};

struct CommandResult {
    CommandStatus status{CommandStatus::Ok};
    std::string message;
};
}

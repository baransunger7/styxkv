#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "styx/utils/string_helpers.hpp"

namespace styx::common {
using DbList = std::vector<std::string>;
using DbSet = std::unordered_set<std::string>;
using DbHash = std::unordered_map<std::string, std::string>;
using DbString = std::string;

using DbValue = std::variant<DbList, DbSet, DbHash, DbString>;

struct Entry {
    DbValue value;
    uint64_t expiresAt{0};
    uint8_t flags{0};
};

enum class DataType {
    List,
    Hash,
    Set,
    String,
    Unknown
};

inline DataType parseDataType(std::string_view rawType) {
    std::string type(rawType);
    type = styx::utils::toLower(type);
    if (type == "list") { return DataType::List; }
    if (type == "hash") { return DataType::Hash; }
    if (type == "set") { return DataType::Set; }
    if (type == "string" || type == "str") { return DataType::String; }

    return DataType::Unknown;
}
enum class CommandStatus {
    Ok,
    Error,
    NotFound,
    Empty
};

struct CommandResult {
    CommandStatus status{CommandStatus::Ok};
    std::string message;
};
}

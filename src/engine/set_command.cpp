#include "styx/engine/set_command.hpp"
#include "styx/utils/overloaded.hpp"

#include <span>
#include <stdexcept>
#include <string>
#include <vector>
#include <variant>

namespace styx::engine {

    styx::common::CommandResult SetCommand::execute(const std::vector<std::string>& args, styx::storage::HashTable& table) {
        if (args.size() < 2) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR Invalid Argument Count. Argument count must be greater than 1."
            };
        }

        std::string key = args[0];
        std::span<const std::string> subArgs(args.data() + 1, args.size() - 1);
        auto entryOpt = table.get(key);

        if (!entryOpt || entryOpt.value() == nullptr) {
            if (args.size() != 2) {
                return styx::common::CommandResult{
                    styx::common::CommandStatus::Error,
                    "ERR key does not exist. Use CREATE to define complex types or SET key value for string."
                };
            }
            table.set(key, styx::common::Entry{args[1]});
            return styx::common::CommandResult{
                styx::common::CommandStatus::Ok,
                "OK"
            };
        }

        auto* entry = entryOpt.value();
        try {
            std::visit(styx::utils::overloaded{
                [&](std::string& str) {
                    if (subArgs.size() != 1) {
                        throw std::runtime_error{"syntax error for string SET"};
                    }
                    str = subArgs[0];
                },
                [&](std::vector<std::string>& list) {
                    SetCommand::updateList(list, subArgs);
                },
                [&](std::unordered_map<std::string, std::string>& hash) {
                    SetCommand::updateHash(hash, subArgs);
                },
                [&](std::unordered_set<std::string>& set) {
                    SetCommand::updateSet(set, subArgs);
                }
            }, entry->value);

            return styx::common::CommandResult{
                styx::common::CommandStatus::Ok,
                "OK"
            };
        } catch (const std::exception& e) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR " + std::string(e.what())
            };
        }
    }

    void SetCommand::updateHash(std::unordered_map<std::string, std::string>& hash, std::span<const std::string> subArgs) {
        if (subArgs.size() % 2 == 1) {
            throw std::runtime_error{"Invalid Argument Count for Hash Update (field-value pairs expected)."};
        }
        if (subArgs.empty()) {
            hash.clear();
            return;
        }
        for (size_t i = 0; i < subArgs.size(); i += 2) {
            hash[subArgs[i]] = subArgs[i + 1];
        }
    }

    void SetCommand::updateList(std::vector<std::string>& list, std::span<const std::string> subArgs) {
        if (subArgs.size() % 2 == 1) {
            throw std::runtime_error{"Invalid Argument Count for List Update (index-value pairs expected)."};
        }
        if (subArgs.empty()) {
            list.clear();
            return;
        }

        for (size_t i = 0; i < subArgs.size(); i += 2) {
            int userIdx = 0;
            try {
                userIdx = std::stoi(subArgs[i]);
            } catch (...) {
                throw std::runtime_error{"Index must be an integer."};
            }

            int targetIdx = -1;
            if (userIdx > 0) {
                targetIdx = userIdx - 1;
            } else if (userIdx < 0) {
                targetIdx = static_cast<int>(list.size()) + userIdx;
            } else {
                throw std::runtime_error{"index 0 is invalid for 1-based indexing."};
            }

            if (targetIdx < 0 || static_cast<size_t>(targetIdx) >= list.size()) {
                throw std::runtime_error{"Index out of range."};
            }

            list[targetIdx] = subArgs[i + 1];
        }
    }

    void SetCommand::updateSet(std::unordered_set<std::string>& set, std::span<const std::string> subArgs) {
        set.clear();
        for (size_t i = 0; i < subArgs.size(); ++i) {
            set.insert(subArgs[i]);
        }
    }

} // namespace styx::engine
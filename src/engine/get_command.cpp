#include "styx/engine/get_command.hpp"
#include "styx/utils/overloaded.hpp"
#include "styx/common/types.hpp"
#include <variant>
#include <sstream>

namespace styx::engine {
    styx::common::CommandResult GetCommand::execute(const std::vector<std::string>& args, styx::storage::HashTable& table) {
        if (args.size() < 1) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "Invalid Argument Count.Argument count must be greater than 0."
            };
        }
        std::string key = args[0];
        auto entryOpt = table.get(key);
        if (!entryOpt) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::NotFound,
                "(nil)"
            };
        }
        auto entry = entryOpt.value();
        std::span<const std::string> subArgs(args.data() + 1, args.size() - 1);

        try {
            std::string formattedOutput = std::visit<std::string>(
                styx::utils::overloaded{
                    [&](const std::string& val) {
                        return formatString(val, subArgs);
                    },
                    [&](const std::vector<std::string>& list) {
                        return formatList(list, subArgs);
                    },
                    [&](const std::unordered_map<std::string, std::string>& hash) {
                        return formatHash(hash, subArgs);
                    },
                    [&](const std::unordered_set<std::string>& set) {
                        return formatSet(set, subArgs);
                    }
                },
                entry->value
            );

            return styx::common::CommandResult{
                styx::common::CommandStatus::Ok,
                formattedOutput
            };
        } catch (const std::exception& e) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                std::string("ERR ") + e.what()
            };
        }
        }

    std::string GetCommand::formatString(const std::string& value,std::span<const std::string> subArgs) const {
        if (!subArgs.empty()) {
            throw std::runtime_error{"String type does not accept sub arguments. "};
        }
        return "\"" + value + "\"";
    }
    std::string GetCommand::formatSet(const std::unordered_set<std::string>& set,std::span<const std::string> subArgs) const {
        if (!subArgs.empty()) {
            throw std::runtime_error{"Set type does not accept sub arguments. "};
        }
        if (set.empty()) {
            return "(empty set)";
        }
        std::ostringstream oss;
        size_t idx = 1;
        for (auto it = set.begin(); it != set.end(); ++it) {
            oss << idx++ << ") \"" << *it << "\"";
            auto next = it;
            if (++next != set.end()) {
                oss << "\n";
            }
        }
        return oss.str();
    }
    std::string GetCommand::formatList(const std::vector<std::string>& list,std::span<const std::string> subArgs) const {
        std::ostringstream oss;
        if (subArgs.empty()) {
            if (list.empty()) {return "(empty list)";}
            for (size_t i = 0; i < list.size(); ++i) {
                oss << (i+1) << ")\"" << list[i] << "\"";
                if (i+1 < list.size()) {oss << "\n";}
            }
            return oss.str();
        }
        for (size_t i = 0; i < subArgs.size(); ++i) {
            int idx = 0;
            try {
                idx = stoi(subArgs[i])-1;
            } catch (...) {
                throw std::runtime_error{"value is not a integer or out of range"};
            }
            if (idx < 0) {
                idx += static_cast<int>(list.size());
            }
            oss << (i + 1) << ") ";
            if (idx >= 0 && static_cast<size_t>(idx) < list.size()) {
                oss << "\"" << list[idx] << "\"";
            } else {
                oss << "(nil)";
            }
            if (i + 1 < subArgs.size()) oss << "\n";
        }
        return oss.str();
    }

    std::string GetCommand::formatHash(const std::unordered_map<std::string,std::string>& hash,std::span<const std::string> subArgs) const{
        std::ostringstream oss;
        if (subArgs.empty()) {
            if (hash.empty()) {return "(empty hash)";}
            size_t idx = 1;
            for (auto it=hash.begin(); it != hash.end(); ++it) {
                oss << idx++ << ")\"" << it->first << "\"\n";
                oss << idx++ << ")\"" << it->second << "\"";
                auto next = it;
                if (++next != hash.end()) oss << "\n";
            }
            return oss.str();
        }
        for (size_t i = 0; i < subArgs.size(); ++i) {
            const auto& field = subArgs[i];
            auto it = hash.find(field);
            oss << (i + 1) << ")";
            if (it != hash.end()) {
                oss << "\"" << it->second << "\"";
            } else {
                oss << "(nil)";
            }
            if (i + 1 < subArgs.size()) oss << "\n";
        }
        return oss.str();
    }

}

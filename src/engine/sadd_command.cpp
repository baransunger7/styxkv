#include "styx/engine/sadd_command.hpp"

#include <span>

namespace styx::engine {
    styx::common::CommandResult SAddCommand::execute(const std::vector<std::string> &args, styx::storage::HashTable &table) {
        if (args.size() < 2) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR wrong number of arguments for 'sadd' command"
            };
        }
        std::string key = args[0];
        auto entryOpt = table.get(key);
        std::span<const std::string> subArgs(args.data()+1, args.size()-1);
        size_t count = 0;
        if (!entryOpt) {
            std::unordered_set<std::string> newSet;
            for (const auto &item : subArgs) {
                auto [it,interested] = newSet.insert(item);
                if (interested) {
                    count++;
                }
            }
            table.set(key,{newSet});
            return styx::common::CommandResult{
                styx::common::CommandStatus::Ok,
                "(integer) " + std::to_string(count)
            };
        }
        auto entry = entryOpt.value();
        auto setPtr = std::get_if<std::unordered_set<std::string>>(&(entry->value));
        if (!setPtr) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR WRONGTYPE Operation against a key holding the wrong kind of value"
            };
        }
        for ( const auto &item : subArgs) {
            auto [it,interested] = setPtr->insert(item);
            if (interested) {
                count++;
            }
        }
        return styx::common::CommandResult{
            styx::common::CommandStatus::Ok,
            "(integer) " + std::to_string(count)
        };
    }
}

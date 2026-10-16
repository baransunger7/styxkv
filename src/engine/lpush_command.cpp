#include "styx/engine/lpush_command.hpp"
#include <vector>

namespace styx::engine {
    styx::common::CommandResult LPushCommand::execute(const std::vector<std::string> &args, styx::storage::HashTable &table) {
        if (args.size() < 2) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR wrong number of arguments for 'lpush' command"
            };
        }
        std::string key = args[0];
        auto entryOpt = table.get(key);
        std::vector<std::string> subArgs(args.begin()+1, args.end());
        if (!entryOpt) {
            std::vector<std::string> newList;
            newList.reserve(subArgs.size());
            for (const auto& item : subArgs) {
                newList.insert(newList.begin(),item);
            }
            table.set(key, styx::common::Entry{newList});
            return styx::common::CommandResult{
                styx::common::CommandStatus::Ok,
                "(integer) " + std::to_string(newList.size())
            };
        }
        auto entry = entryOpt.value();
        auto listPtr = std::get_if<std::vector<std::string>>(&(entry->value));
        if (!listPtr) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR WRONGTYPE Operation against a key holding the wrong kind of value"
            };
        }
        for (const auto& item : subArgs) {
            listPtr->insert(listPtr->begin(), item);
        }
        return styx::common::CommandResult{
            styx::common::CommandStatus::Ok,
            "(integer) " + std::to_string(listPtr->size())
        };
    }
}

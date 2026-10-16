#include "styx/engine/srem_command.hpp"


#include <span>

namespace styx::engine {
    styx::common::CommandResult SRemCommand::execute(const std::vector<std::string> &args, styx::storage::HashTable &table) {
        if (args.size() < 2) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR wrong number of arguments for 'srem' command"
            };
        }
        std::string key = args[0];
        auto entryOpt = table.get(key);
        std::span<const std::string> subArgs(args.data()+1, args.size()-1);
        size_t count = 0;
        if (!entryOpt) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Ok,
                "(integer) 0"
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
            count += setPtr->erase(item);
        }
        if (setPtr->empty()) {
            table.del(key);
        }
        return styx::common::CommandResult{
            styx::common::CommandStatus::Ok,
            "(integer) " + std::to_string(count)
        };
    }
}


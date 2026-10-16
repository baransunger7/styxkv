#include "styx/engine/rpush_command.hpp"

namespace styx::engine {
    styx::common::CommandResult RPushCommand::execute(const std::vector<std::string> &args, styx::storage::HashTable &table) {
        if (args.size() < 2) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR wrong number of arguments for 'rpush' command"
            };
        }
        std::string key = args[0];
        auto entryOpt = table.get(key);
        std::vector<std::string> subArgs(args.begin()+1, args.end());
        if (!entryOpt) {
            table.set(key, styx::common::Entry{subArgs});
            return styx::common::CommandResult{
                styx::common::CommandStatus::Ok,
                "(integer) " + std::to_string(subArgs.size())
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
        listPtr->insert(listPtr->end(), subArgs.begin(), subArgs.end());
        return styx::common::CommandResult{
            styx::common::CommandStatus::Ok,
            "(integer) " + std::to_string(listPtr->size())
        };
    }
}

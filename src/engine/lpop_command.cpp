#include "styx/engine/lpop_command.hpp"

namespace styx::engine {
    styx::common::CommandResult LPopCommand::execute(const std::vector<std::string> &args, styx::storage::HashTable &table) {
        if (args.size() != 1) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR wrong number of arguments for 'lpop' command"
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
        auto listPtr = std::get_if<std::vector<std::string>>(&(entry->value));
        if (!listPtr) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR WRONGTYPE Operation against a key holding the wrong kind of value"
            };
        }
        if (listPtr->empty()) {
            table.del(key);
            return styx::common::CommandResult{
                styx::common::CommandStatus::NotFound,
                "(nil)"
            };
        }
        std::string val = listPtr->front();
        listPtr->erase(listPtr->begin());
        if (listPtr->empty()) {
            table.del(key);
        }
        return styx::common::CommandResult{
            styx::common::CommandStatus::Ok,
            std::string("\"") + val + "\""
        };
    }
}
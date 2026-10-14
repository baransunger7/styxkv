#include "styx/engine/exists_command.hpp"


namespace styx::engine {
styx::common::CommandResult ExistsCommand::execute(const std::vector<std::string> &args, styx::storage::HashTable &table) {
    if (args.empty()) {
        return styx::common::CommandResult{
            styx::common::CommandStatus::Error,
            "ERR wrong number of arguments for 'exists' command."
        };
    }
    int count = 0;
    for (const auto &key : args) {
        auto entryOpt = table.get(key);
        if (entryOpt) {
            count++;
        }
    }
    return styx::common::CommandResult{
        styx::common::CommandStatus::Ok,
        "(integer) " + std::to_string(count)
    };
}
}

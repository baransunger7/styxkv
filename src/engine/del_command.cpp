
#include "styx/engine/del_command.hpp"
#include "styx/common/types.hpp"

namespace styx::engine {

styx::common::CommandResult DelCommand::execute(const std::vector<std::string> &args, styx::storage::HashTable& table) {
    if (args.empty()) {
        return styx::common::CommandResult{
            styx::common::CommandStatus::Error,
            "ERR wrong number of arguments for 'del' command"
        };
    }
    size_t deletedCount = 0;
    for (const auto& key : args) {
        if (table.del(key)) {
            ++deletedCount;
        }
    }
    return styx::common::CommandResult{
        styx::common::CommandStatus::Ok,
        "(integer) " + std::to_string(deletedCount)
    };
}
}

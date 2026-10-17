#include "styx/engine/ttl_command.hpp"

#include "styx/utils/time_utils.hpp"

namespace styx::engine {
    styx::common::CommandResult TTLCommand::execute(const std::vector<std::string> &args, styx::storage::HashTable &table) {
        if (args.size() != 1) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR wrong number of arguments for 'ttl' command"
            };
        }
        std::string key = args[0];
        auto entryOpt = table.get(key);
        if (!entryOpt) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Ok,
                "(integer) -2"
            };
        }
        auto entry = entryOpt.value();
        if (entry->expiresAt == 0) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Ok,
                "(integer) -1"
            };
        }
        uint64_t now = utils::currentTimeMillis();
        if (now > entry->expiresAt) {
            table.del(key);
            return styx::common::CommandResult{
                styx::common::CommandStatus::Ok,
                "(integer) -2"
            };
        }
        uint64_t remainingMs = entry->expiresAt - now;
        int64_t remainingSeconds = static_cast<int64_t>((remainingMs+999)/1000);

        return styx::common::CommandResult{
            styx::common::CommandStatus::Ok,
            "(integer) " + std::to_string(remainingSeconds)
        };
    }
}

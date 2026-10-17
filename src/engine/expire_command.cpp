#include "styx/engine/expire_command.hpp"

#include <stdexcept>

#include "styx/utils/time_utils.hpp"

namespace styx::engine {
    styx::common::CommandResult ExpireCommand::execute(const std::vector<std::string> &args, styx::storage::HashTable &table) {
        if (args.size() != 2) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR wrong number of arguments for 'expire' command"
            };
        }
        std::string key = args[0];
        int64_t seconds;
        try {
            seconds =  std::stoll(args[1]);
        }catch (const std::exception&) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR value is not an integer or out of range"
            };
        }
        if (seconds <= 0) {
            bool deleted = table.del(key);
            return styx::common::CommandResult{
                styx::common::CommandStatus::Ok,
                "(integer) " + std::to_string(deleted ? 1 : 0)
            };
        }

        auto entryOpt = table.get(key);
        if (!entryOpt) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Ok,
                "(integer) 0"
            };
        }
        auto entry = entryOpt.value();
        uint64_t ttlMs = static_cast<uint64_t>(seconds) * 1000;
        entry->expiresAt = utils::currentTimeMillis() + ttlMs;
        return styx::common::CommandResult{
            styx::common::CommandStatus::Ok,
            "(integer) 1"
        };

    }
}

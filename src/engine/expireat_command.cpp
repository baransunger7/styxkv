#include "styx/engine/expireat_command.hpp"

#include <chrono>
#include <cstdint>

namespace styx::engine {
    styx::common::CommandResult ExpireAtCommand::execute(const std::vector<std::string> &args, styx::storage::HashTable &table) {
        if (args.size() != 2) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR wrong number of arguments for 'expireat' command"
            };
        }
        std::string key = args[0];
        uint64_t targetTimestamp = 0;

        try {
            long long parsed = std::stoll(args[1]);
            if (parsed < 0) {
                return styx::common::CommandResult{
                    styx::common::CommandStatus::Error,
                    "ERR invalid expire time,must be positive"
                };
            }
            targetTimestamp = static_cast<uint64_t>(parsed);
        }catch (...) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR value is not an integer or out of range"
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
        auto now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        uint64_t currentTimestamp = static_cast<uint64_t>(now);
        if (currentTimestamp >= targetTimestamp) {
            table.del(key);
            return styx::common::CommandResult{
                styx::common::CommandStatus::Ok,
                "(integer) 1"
            };
        }
        entry->expiresAt = targetTimestamp;
        return styx::common::CommandResult{
            styx::common::CommandStatus::Ok,
            "(integer) 1"
        };

    }
}

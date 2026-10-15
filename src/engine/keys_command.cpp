#include "styx/engine/keys_command.hpp"
#include <sstream>

namespace styx::engine {

    styx::common::CommandResult KeysCommand::execute(const std::vector<std::string> &args, styx::storage::HashTable &table) {
        if (args.size() > 1) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR wrong number of arguments for 'keys' command"
            };
        }
        if (!args.empty() && args[0] != "*") {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR only '*' pattern is supported in current version"
            };
        }
        std::vector<std::string> keys = table.keys();
        if (keys.empty()) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Ok,
                "(empty list)"
            };
        }
        std::ostringstream oss;
        for (size_t i = 0; i < keys.size(); ++i) {
            oss << (i + 1) << ") \"" << keys[i] << "\"";
            if (i + 1 < keys.size()) {
                oss << "\n";
            }
        }

        return styx::common::CommandResult{
            styx::common::CommandStatus::Ok,
            oss.str()
        };
    }
}
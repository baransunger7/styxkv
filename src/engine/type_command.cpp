#include "styx/engine/type_command.hpp"
#include "styx/utils/overloaded.hpp"

namespace styx::engine {
    styx::common::CommandResult TypeCommand::execute(const std::vector<std::string> &args, styx::storage::HashTable& table) {
        if (args.size() != 1) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR wrong number of arguments for 'type' command"
            };
        }
        std::string key = args[0];
        auto entryOpt = table.get(key);
        if (!entryOpt) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::NotFound,
                "none"
            };
        }
        auto entry = entryOpt.value();
        std::string typeName = std::visit(styx::utils::overloaded{
    [](const styx::common::DbString&){return "string";},
    [](const styx::common::DbList&){return "list";},
    [](const styx::common::DbSet&){return "set";},
    [](const styx::common::DbHash&){return "hash";},
        },entry->value);
        return styx::common::CommandResult{
            styx::common::CommandStatus::Ok,
typeName
        };
    }
}

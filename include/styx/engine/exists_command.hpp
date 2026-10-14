#include "styx/engine/command.hpp"


namespace styx::engine {
class ExistsCommand : public Command {
public:
    ExistsCommand() = default;
    styx::common::CommandResult execute(const std::vector<std::string> &args, styx::storage::HashTable &table) override;
};
}

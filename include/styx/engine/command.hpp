#pragma once
#include "styx/common/types.hpp"

namespace styx::engine {

class Command {
public:
    virtual ~Command() = default;
    [[nodiscard]] virtual styx::common::CommandResult execute();
};
}

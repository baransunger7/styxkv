#pragma once
#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

namespace styx::utils {
    inline std::string toLower(std::string str) {
        std::transform(str.begin(), str.end(), str.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return str;
    }
    std::vector<std::string> tokenize(std::string_view str);

    enum class State : uint8_t{
        Delimiter,
        Normal,
        Quoted,
    };
}

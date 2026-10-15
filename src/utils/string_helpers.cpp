#include "styx/utils/string_helpers.hpp"

#include <stdexcept>
#include <vector>

namespace styx::utils {
    std::vector<std::string> tokenize(std::string_view str) {
        std::vector<std::string> tokens;
        std::string currentToken;
        State currentState = State::Delimiter;
        bool escapeActive = false;

        for (char c : str) {
            if (escapeActive) {
                currentToken += c;
                escapeActive = false;
                continue;
            }
            if (c == '\\') {
                escapeActive = true;
                continue;
            }
            if (currentState == State::Delimiter) {
                if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {continue;}
                if (c == '"') {
                    currentToken.clear();
                    currentState = State::Quoted;
                    continue;
                }
                currentToken.clear();
                currentToken += c;
                currentState = State::Normal;
                continue;
            }
            if (currentState == State::Normal) {
                if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
                    tokens.push_back(std::move(currentToken));
                    currentToken.clear();
                    currentState = State::Delimiter;
                    continue;
                }
                if (c == '"') {
                    currentState = State::Quoted;
                    continue;
                }
                currentToken += c;
                continue;
            }
            if (currentState == State::Quoted) {
                if (c == '"') {
                    tokens.push_back(std::move(currentToken));
                    currentToken.clear();
                    currentState = State::Delimiter;
                    continue;
                }
                currentToken += c;
            }
        }
        if (currentState == State::Quoted) {throw std::runtime_error("unclosed quote");}
        if (escapeActive) {throw std::runtime_error("escape is not completed");}
        if (currentState == State::Normal && !currentToken.empty()){tokens.push_back(std::move(currentToken));}
        return tokens;
    }
}

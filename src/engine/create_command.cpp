#include "styx/engine/create_command.hpp"
#include "styx/utils/string_helpers.hpp"

namespace styx::engine {
    styx::common::CommandResult CreateCommand::execute(const std::vector<std::string>& args, styx::storage::HashTable &table) {
        if (args.size() < 2) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR Invalid Argument Count.Argument count must be greater than 1."
            };
        }
        styx::common::DataType targetType = styx::common::parseDataType(styx::utils::toLower(args[0]));
        switch (targetType) {
            case styx::common::DataType::String:
                return createString(args, table);
            case styx::common::DataType::Set:
                return createSet(args, table);
            case styx::common::DataType::Hash:
                return createHash(args, table);
            case styx::common::DataType::List:
                return createList(args, table);
            case styx::common::DataType::Unknown:
            default:
                return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR Unknown DataType."
            };
        }
    }
    styx::common::CommandResult CreateCommand::createString(const std::vector<std::string> &args, styx::storage::HashTable &table) {
        if (args.size() != 3) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR Invalid Argument Count.Argument count must be 3."
            };
        }
        std::string key = args[1];
        styx::common::Entry entry{args[2]};
        if (table.get(key)) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR Key Already Exists."
            };
        }
        table.set(key,entry);
        return styx::common::CommandResult{
            styx::common::CommandStatus::Ok,
            "OK String Value Created."
        };
    }
    styx::common::CommandResult CreateCommand::createList(const std::vector<std::string> &args, styx::storage::HashTable &table) {
        if (args.size() < 2) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR Invalid Argument Count.Argument count must be greater than 1."
            };
        }
        std::string key = args[1];
        std::vector<std::string> list(args.begin()+2, args.end());
        styx::common::Entry entry{list};
        if (table.get(key)) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR Key Already Exists."
            };
        }
        table.set(key,entry);
        return styx::common::CommandResult{
            styx::common::CommandStatus::Ok,
            "OK List  Created."
        };

    }
    styx::common::CommandResult CreateCommand::createSet(const std::vector<std::string> &args, styx::storage::HashTable &table) {
        if (args.size() < 2) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR Invalid Argument Count.Argument count must be greater than 1."
            };
        }
        std::string key = args[1];
        std::unordered_set<std::string> set(args.begin()+2, args.end());
        styx::common::Entry entry{set};
        if (table.get(key)) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR Key Already Exists."
            };
        }
        table.set(key,entry);
        return styx::common::CommandResult{
            styx::common::CommandStatus::Ok,
            "OK Set  Created."
        };
    }
    styx::common::CommandResult CreateCommand::createHash(const std::vector<std::string> &args, styx::storage::HashTable &table) {
        if (args.size() < 2) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR Invalid Argument Count.Argument count must be greater than 1."
            };
        }
        if (args.size()%2 == 1 ) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR Invalid Argument Count.Argument count must be even."
            };
        }
        std::string key = args[1];
        if (table.get(key)) {
            return styx::common::CommandResult{
                styx::common::CommandStatus::Error,
                "ERR Key Already Exists."
            };
        }
        std::unordered_map<std::string,std::string> hash;
        for (size_t i = 2; i < args.size(); i += 2) {
            hash[args[i]] = args[i + 1];
        }
        styx::common::Entry entry{hash};
        table.set(key,entry);
        return styx::common::CommandResult{
            styx::common::CommandStatus::Ok,
            "OK Hash Created."
        };
    }
}

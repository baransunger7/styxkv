#include "styx/storage/persistence_manager.hpp"
#include "styx/engine/command_dispatcher.hpp"


namespace styx::storage {

    PersistenceManager::PersistenceManager(const std::string& aofPath):
        aofPath_{aofPath},aofStream_{aofPath,std::ios::app} {
        if (!aofStream_.is_open()) {
            throw std::runtime_error("Failed to open AOF file: " + aofPath_);
        }
    }

    void PersistenceManager::logCommand(const std::string &rawCommandLine) {
        if (aofStream_.is_open()) {
            aofStream_ << rawCommandLine << "\n";
            aofStream_.flush();
        }
    }

    void PersistenceManager::recover(styx::storage::HashTable& table, styx::engine::CommandDispatcher& dispatcher) {
        std::string line;
        std::ifstream recoveryStream{aofPath_,std::ios::in};
        if (!recoveryStream.is_open()) {return;}
        while (std::getline(recoveryStream,line)) {
            if (line.empty()){continue;}
            dispatcher.dispatch(line,table);
        }
    }

    void PersistenceManager::flush() {
        if (aofStream_.is_open()) {
            aofStream_.flush();
        }
    }

    PersistenceManager::~PersistenceManager() {
        flush();
        if (aofStream_.is_open()) {
            aofStream_.close();
        }
    }
}

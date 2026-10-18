#include <fstream>
namespace styx::engine {
    class CommandDispatcher;
}

namespace  styx::storage {
class HashTable;


class PersistenceManager {
public:
    explicit PersistenceManager(const std::string& aofPath);
    ~PersistenceManager();

    void recover(styx::storage::HashTable& table,styx::engine::CommandDispatcher& dispatcher);
    void logCommand(const std::string& rawCommandLine);
    void flush();

private:
    std::string aofPath_;
    std::ofstream aofStream_;
};
}
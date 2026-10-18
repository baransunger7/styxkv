#pragma once

#include <functional>
#include <optional>
#include <string>

#include "styx/common/types.hpp"

using DeleteListener = std::function<void(const std::string&)>;
namespace styx::storage {

struct HashNode {
    std::string key;
    styx::common::Entry entry;
    HashNode* next{nullptr};

    HashNode(std::string k, styx::common::Entry e, HashNode* n = nullptr)
        :key{std::move(k)}, entry{std::move(e)}, next{n}{}
};

class HashTable {
public:
    explicit HashTable(size_t initialCapacity=1024);
    ~HashTable();
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    void set(const std::string& key,common::Entry entry);
    std::optional<styx::common::Entry*> get(const std::string& key);
    bool del(const std::string& key);
    [[nodiscard]] std::vector<std::string> keys() const;
    void setOnKeyDeleted(DeleteListener listener);


private:
    std::vector<HashNode*> buckets_;
    DeleteListener onKeyDeleted_;
    size_t capacity_;
    size_t size_{0};
    uint64_t hashFunc(const std::string& key) const;
    size_t getBucketIndex(const std::string& key) const;
};
}

#pragma once

#include <optional>
#include <string>

#include "styx/common/types.hpp"

namespace styx::storage {

struct HashNode {
    std::string key;
    styx::common::DbValue value;
    HashNode* next{nullptr};

    HashNode(std::string k, styx::common::DbValue v, HashNode* n = nullptr)
        :key{std::move(k)}, value{std::move(v)}, next{n}{}
};

class HashTable {
public:
    explicit HashTable(size_t initialCapacity=1024);
    ~HashTable();
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    void set(const std::string& key,common::DbValue value);
    std::optional<styx::common::DbValue> get(const std::string& key) const;
    bool del(const std::string& key);

private:
    std::vector<HashNode*> buckets_;
    size_t capacity_;
    size_t size_{0};
    uint64_t hashFunc(const std::string& key) const;
    size_t getBucketIndex(const std::string& key) const;
};
}

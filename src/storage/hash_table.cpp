#include "styx/storage/hash_table.hpp"
#include <bit>

namespace styx::storage {
    HashTable::HashTable(size_t initialCapacity) {
        if (initialCapacity == 0 || (initialCapacity & (initialCapacity - 1)) != 0) {
            capacity_ = std::bit_ceil(initialCapacity);
        }else {
            capacity_ = initialCapacity;
        }
        buckets_.resize(capacity_,nullptr);
    }
    HashTable::~HashTable() {
        for (size_t i = 0; i < capacity_; i++) {
            auto curr = buckets_[i];
            while (curr != nullptr) {
                auto nextNode = curr->next;
                delete curr;
                curr = nextNode;
            }
        }
    }

    size_t HashTable::getBucketIndex(const std::string &key) const {
        uint64_t hashedKey = hashFunc(key);
        return hashedKey & (capacity_ - 1);
    }

    std::optional<styx::common::DbValue> HashTable::get(const std::string &key) const {
        size_t index = getBucketIndex(key);
        auto curr = buckets_[index];
        while (curr != nullptr) {
            if (curr->key == key) {
                return curr->value;
            }
            curr = curr->next;
        }
        return std::nullopt;
    }

    void HashTable::set(const std::string &key, common::DbValue value) {
        size_t index = getBucketIndex(key);
        auto curr = buckets_[index];
        while (curr != nullptr) {
            if (curr->key == key) {
                curr->value = value;
                return;
            }
            curr = curr->next;
        }
        buckets_[index] = new HashNode(key, value, buckets_[index]);
        size_++;
    }

    bool HashTable::del(const std::string &key) {
        size_t index = getBucketIndex(key);
        if (buckets_[index] == nullptr) {
            return false;
        }
        if (buckets_[index]->key == key) {
            auto toDelete = buckets_[index];
            buckets_[index] = toDelete->next;
            delete toDelete;
            size_--;
            return true;
        }
        auto prev = buckets_[index];
        auto curr = buckets_[index]->next;
        while (curr != nullptr) {
            if (curr->key == key) {
                prev->next = curr->next;
                delete curr;
                size_--;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }
}

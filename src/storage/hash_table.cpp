#include "styx/storage/hash_table.hpp"
#include <bit>

#define XXH_INLINE_ALL
#include "styx/utils/time_utils.hpp"
#include "styx/utils/xxhash.h"

namespace styx::storage {
    HashTable::HashTable(size_t initialCapacity) {
        if (initialCapacity < 16) {
            initialCapacity = 16;
        }
        capacity_ = std::bit_ceil(initialCapacity);
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

    std::optional<styx::common::Entry*> HashTable::get(const std::string &key)  {
        size_t index = getBucketIndex(key);

        auto curr = buckets_[index];
        while (curr != nullptr) {
            if (curr->key == key) {
                if (curr->entry.expiresAt > 0 && utils::currentTimeMillis() >= curr->entry.expiresAt) {
                    this->del(curr->key);
                    return std::nullopt;
                }
                return &(curr->entry);
            }
            curr = curr->next;
        }
        return std::nullopt;
    }

    void HashTable::set(const std::string &key, common::Entry entry) {
        size_t index = getBucketIndex(key);
        auto curr = buckets_[index];
        while (curr != nullptr) {
            if (curr->key == key) {
                curr->entry = entry;
                return;
            }
            curr = curr->next;
        }
        buckets_[index] = new HashNode(key, entry, buckets_[index]);
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

    std::vector<std::string> HashTable::keys() const {
        std::vector<std::string> keys;
        keys.reserve(size_);
        for (auto& it : buckets_) {
            auto curr = it;
            while (curr != nullptr) {
                keys.push_back(curr->key);
                curr = curr->next;
            }
        }
        return keys;
    }

    uint64_t HashTable::hashFunc(const std::string &key) const {
        return XXH64(key.data(), key.size(),0);
    }
}

//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_replacer.h"

namespace bustub {

LRUReplacer::LRUReplacer(size_t num_pages) {
    capacity = num_pages;
}

LRUReplacer::~LRUReplacer() = default;

bool LRUReplacer::Victim(frame_id_t *frame_id) {
    const std::lock_guard<mutex_t> guard(mutex_);

    if (lst.empty()) {
        return false;
    }

    frame_id_t f = lst.front();
    lst.pop_front();

    auto table_it = table_.find(f);

    if (table_it != table_.end()) {
        *frame_id = f;
        table_.erase(table_it);
        return true;
    }
    return false;
}

void LRUReplacer::Pin(frame_id_t frame_id) {
    const std::lock_guard<mutex_t> guard(mutex_);

    auto table_it = table_.find(frame_id);
    if (table_it != table_.end()) {
        lst.erase(table_it->second);
        table_.erase(table_it);
    }
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
    const std::lock_guard<mutex_t> guard(mutex_);

    if (table_.size() >= capacity) {
        return;
    }

    auto table_it = table_.find(frame_id);
    if (table_it != table_.end()) {
        return;
    }

    lst.push_back(frame_id);
    table_.emplace(frame_id, std::prev(lst.end(), 1));
}

size_t LRUReplacer::Size() {
    const std::lock_guard<mutex_t> guard(mutex_);

    return table_.size();
}

}  // namespace bustub

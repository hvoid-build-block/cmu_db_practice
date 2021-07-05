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
  limit_pages = num_pages;
  cur_page_size = 0;

  head = std::make_shared<Node>();
  tail = std::make_shared<Node>();
  head->next = tail;
  tail->prev = head;
}

LRUReplacer::~LRUReplacer() = default;

bool LRUReplacer::Victim(frame_id_t *frame_id) {
  lock_guard<std::mutex> lock(latch);
  if (content_map.empty()) {
    return false;
  }

  shared_ptr<Node> last = tail->prev;
  *frame_id = last->val;
  tail->prev = last->prev;
  last->prev->next = tail;
  content_map.erase(*frame_id);
  return false;
}

void LRUReplacer::Pin(frame_id_t frame_id) {
  lock_guard<mutex> lock(latch);
  if (content_map.find(frame_id) == content_map.end()) {
    return;
  }

  shared_ptr<Node> cur = content_map[frame_id];
  cur->prev->next = cur->next;
  cur->next->prev = cur->prev;
  content_map.erase(frame_id);
  return;
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
  lock_guard<mutex> lock(latch);
  shared_ptr<Node> cur;

  if (content_map.find(frame_id) != content_map.end()) {
    cur = content_map[frame_id];
    shared_ptr<Node> prev = cur->prev;
    shared_ptr<Node> succ = cur->next;
    prev->next = succ;
    succ->prev = prev;
  } else {
    cur = std::make_shared<Node>(frame_id);
  }

  shared_ptr<Node> first = head->next;
  first->prev = cur;
  cur->prev = head;
  head->next = cur;
  cur->next = first;
  content_map[frame_id] = cur;
  return;
}

size_t LRUReplacer::Size() { return content_map.size(); }

}  // namespace bustub

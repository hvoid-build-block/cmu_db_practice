//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.h
//
// Identification: src/include/buffer/lru_replacer.h
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <list>
#include <mutex>  // NOLINT
#include <vector>
#include <unordered_map>

#include "buffer/replacer.h"
#include "common/config.h"

using std::lock_guard;
using std::mutex;
using std::shared_ptr;

namespace bustub {

/**
 * LRUReplacer implements the lru replacement policy, which approximates the Least Recently Used policy.
 */
class LRUReplacer : public Replacer {
  struct Node {
      Node(){};
      Node(frame_id_t val): val(val){};
      frame_id_t val;
      shared_ptr<Node> prev;
      shared_ptr<Node> next;
    };
 public:
  /**
   * Create a new LRUReplacer.
   * @param num_pages the maximum number of pages the LRUReplacer will be required to store
   */
  explicit LRUReplacer(size_t num_pages);

  /**
   * Destroys the LRUReplacer.
   */
  ~LRUReplacer() override;

  bool Victim(frame_id_t *frame_id) override;

  void Pin(frame_id_t frame_id) override;

  void Unpin(frame_id_t frame_id) override;

  size_t Size() override;

 private:
  // TODO(student): implement me!
  shared_ptr<Node> head;
  shared_ptr<Node> tail;
  std::unordered_map<frame_id_t, shared_ptr<Node>> content_map;
  mutable mutex latch;
  size_t cur_page_size;
  size_t limit_pages;
};

}  // namespace bustub

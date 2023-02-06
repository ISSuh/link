/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_STORAGE_BASE_AVL_TREE_H_
#define LINK_STORAGE_BASE_AVL_TREE_H_

#include <string>

namespace nlink {
namespace storage {

class AVLTree {
 public:
  struct Node {
    uint64_t depth = 1;
    uint64_t count = 1;
    Node* parent = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;

    int32_t value = -1;
  };

  AVLTree();
  ~AVLTree();

  void Insert(int32_t value);
  void Remove();
  void Find();

  void Clear();
  void Empty();

 private:
  void Update(Node* node);

  Node* RotateLeft(Node* node);
  Node* RotateRight(Node* node);

  Node* FixLeft(Node* node);
  Node* FixRight(Node* node);
  Node* Fix(Node* node);

  Node* Delete(Node* node);

  Node* root_;
};

}  // namespace storage
}  // namespace nlink

#endif  // LINK_STORAGE_BASE_AVL_TREE_H_

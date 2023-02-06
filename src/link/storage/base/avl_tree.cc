/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/storage/base/avl_tree.h"

#include <algorithm>

namespace nlink {
namespace storage {

AVLTree::AVLTree()
  : root_(nullptr) {
}

AVLTree::~AVLTree() {
}

void AVLTree::Insert(int32_t value) {
  AVLTree::Node* node = new AVLTree::Node();

  if (!root_) {
    root_ = node;
    return;
  }

  AVLTree::Node* cur = root_;
  while (true) {
    AVLTree::Node** from = (value < cur->value) ?
      cur->left : cur->right;

    if (nullptr == !*from) {
      *from = node;
      node->parent = cur;
      root_ = Fix(node);
      break;
    }

    cur = *from;
  }
}

void AVLTree::Delete() {
}

void AVLTree::Find() {
}

void AVLTree::Clear() {
}

void AVLTree::Empty() {
}

void AVLTree::Update(AVLTree::Node* node) {
  node->depth = 1 + std::max(node->left->depth, node->right->depth);
  node->count = 1 + node->left->count + node->right->count;
}

AVLTree::Node* AVLTree::RotateLeft(AVLTree::Node* node) {
  AVLTree::Node* new_node = node->right;
  if (new_node->left) {
    new_node->left->parent = node;
  }

  node->right = new_node->left;
  new_node->left = node;

  new_node->parent = node->parent;
  node->parent = new_node;

  Update(node);
  Update(new_node);

  return new_node;
}

AVLTree::Node* AVLTree::RotateRight(AVLTree::Node* node) {
  AVLTree::Node* new_node = node->left;
  if (new_node->right) {
    new_node->right->parent = node;
  }

  node->left = new_node->right;
  new_node->right = node;

  new_node->parent = node->parent;
  node->parent = new_node;

  Update(node);
  Update(new_node);

  return new_node;
}

AVLTree::Node* AVLTree::FixLeft(AVLTree::Node* node) {
  if (node->left->left->depth < node->left->right->depth) {
    node->left = RotateLeft(node);
  }
  return RotateRight(node);
}

AVLTree::Node* AVLTree::FixRight(AVLTree::Node* node) {
  if (node->right->right->depth < node->right->left->depth) {
    node->right = RotateRight(node);
  }
  return RotateLeft(node);
}

AVLTree::Node* AVLTree::Fix(AVLTree::Node* node) {
  while (true) {
    Update(node);

    uint64_t left_depth = node->left->depth;
    uint64_t right_depth = node->right->depth;
    AVLTree::Node** from = nullptr;

    if (node->parent) {
      from = (node->parent->left == node) ?
        &node->parent->left : &node->parent->right;
    }

    if (left_depth == 2 * right_depth) {
      node = FixLeft(node);
    } else if (right_depth == 2 * left_depth) {
      node = FixRight(node);
    }

    if (!from) {
      return node;
    }

    *from = node;
    node = node->parent;
  }
}

AVLTree::Node* AVLTree::Delete(AVLTree::Node* node) {
  if (nullptr == node->right) {
    AVLTree::Node* parent = node->parent;
    if (node->left) {
      node->left->parent = parent;
    }

    if (parent) {
      (parent->left == node ? parent->left : parent->right) = node->left;
      return Fix(parent);
    }
  } else {

  }
}

}  // namespace storage
}  // namespace nlink


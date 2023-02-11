#include <iostream>
#include <vector>

const int maxKeys = 3;  // Maximum number of keys per node
const int minKeys = maxKeys / 2;  // Minimum number of keys per node

struct BPlusTreeNode {
  std::vector<int> keys;  // Array of keys
  std::vector<BPlusTreeNode*> children;  // Array of child pointers
  bool isLeaf;  // Flag to indicate if the node is a leaf node

  // Constructor
  BPlusTreeNode() {
    keys.resize(maxKeys + 1);
    children.resize(maxKeys + 2);
    isLeaf = true;
  }

  // Insert a key-value pair into the node
  void insert(int key) {
    int i = keys.size() - 1;
    while (i >= 0 && keys[i] > key) {
      keys[i + 1] = keys[i];
      i--;
    }
    keys[i + 1] = key;
  }

  // Split the node into two separate nodes
  BPlusTreeNode* split() {
    BPlusTreeNode* newNode = new BPlusTreeNode();
    int midIndex = keys.size() / 2;
    for (int i = midIndex; i < keys.size(); i++) {
      newNode->keys[i - midIndex] = keys[i];
    }
    keys.resize(midIndex);
    return newNode;
  }
};

class BPlusTree {
 public:
  BPlusTreeNode* root;

  // Constructor
  BPlusTree() { root = new BPlusTreeNode(); }

  // Insert a key-value pair into the tree
  void insert(int key) {
    BPlusTreeNode* node = root;
    while (!node->isLeaf) {
      int i = node->keys.size() - 1;
      while (i >= 0 && key < node->keys[i]) {
        i--;
      }
      node = node->children[i + 1];
    }
    node->insert(key);
    if (node->keys.size() > maxKeys) {
      BPlusTreeNode* newNode = node->split();
      node->children[node->children.size() - 1] = newNode;
      node->isLeaf = false;
    }
  }

  // Delete a key-value pair from the tree
  void remove(int key) {
    BPlusTreeNode* node = root;
    while (!node->isLeaf) {
      int i = node->keys.size() - 1;
      while (i >= 0 && key < node->keys[i]) {
        i--;
      }
      node = node->children[i + 1];
    }
    int i = 0;
    while (i < node->keys.size() && node->keys[i] != key) {
      i++;
    }
    if (i < node->keys.size()) {
      node->keys.erase(node->keys.begin() + i);
      if (node->keys.size() < minKeys) {
        merge(node);
      }
    }
  }

  // Search for a key-value pair in the tree
  bool search(int key) {
    BPlusTreeNode* node = root;
    while (!node->isLeaf) {
      int i = node->keys.size() - 1;
      while (i >= 0 && key < node->keys[i]) {
        i--;
      }
      node = node->children[i + 1];
    }
    int i = 0;
    while (i < node->keys.size() && node->keys[i] != key) {
      i++;
    }
    return i < node->keys.size();
  }
};

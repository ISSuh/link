/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/http/routeing.h"

namespace nlink {
namespace net {
namespace http {

const char* kPathDelimiter = "/";
const size_t kPathDelimiterSize = 1;

void ParseNextPath(
  const std::string& origin,
  std::string* current_path,
  std::string* remaind_path) {
  if (origin.empty()) {
    return;
  }

  size_t pos = origin.find(kPathDelimiter);
  if (pos == std::string::npos) {
    return;
  }

  std::string token = origin.substr(0, pos);
  std::string others = origin.substr(pos);
  if (token == kPathDelimiter) {
    return ParseNextPath(others, current_path, remaind_path);
  }

  *current_path = origin.substr(0, pos);
  *remaind_path = origin.substr(pos);
}

// Routing::RoutingNode::RoutingNode(
//   const std::string& path,
//   std::function<void()> handler)
//   : path_(path),
//     handler_(handler) {
// }

// Routing::RoutingNode::~RoutingNode() = default;

// Routing::RoutingNode* Routing::RoutingNode::Find(
//   const std::string& path) const {
//   if (sub_nodes_.empty()) {
//     return nullptr;
//   }


//   for (const auto& item : sub_nodes_) {
//     const std::string node_path = item.first;
//     Find();
//   }
// }

// void Routing::RoutingNode::Insert(
//   const std::string& path, std::unique_ptr<RoutingNode> node) {

// }

Routing::Routing()
  : root_(nullptr) {
}

Routing::~Routing() = default;

void Routing::RegistHandler(
  const std::string& path,
  std::function<void()> handler) {
  
}

void Routing::SearchPath(const std::string& path) {
  size_t pos = 0;
  std::string token("");

  pos = path.find(kPathDelimiter);
  if (pos == std::string::npos) {
    
    return;
  }
}

void Routing::RecursiveInsert(
  const std::string& path,
  std::function<void()> handler,
  std::unique_ptr<Routing::RoutingNode>& current_node) {
  
  std::string current_path;
  std::string other_path;
  ParseNextPath(path, &current_path, &other_path);

  
}

std::unique_ptr<Routing::RoutingNode> Routing::CreateNewRoutingNode(
  const std::string& path,
  std::function<void()> handler) {
  return std::make_unique<Routing::RoutingNode>(path, handler);
}

}  // namespace http
}  // namespace net
}  // namespace nlink

/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/http/routeing.h"

#include <sstream>
#include <utility>

#include "link/base/logging.h"

namespace nlink {
namespace net {
namespace http {

const char* kRootPath = "/";
const char kPathDelimiter = '/';
const size_t kPathDelimiterSize = 1;

void SplitPathBySlash(
  const std::string& origin,
  std::vector<std::string>* splited_path) {
  if (origin.empty()) {
    return;
  }

  std::stringstream stream(origin);
  std::string item;
  while (std::getline(stream, item, kPathDelimiter)) {
    if (!item.empty()) {
      splited_path->emplace_back(item);
    }
  }
}

Routing::RoutingNode::RoutingNode(
  const std::string& node_path,
  std::function<void()> node_handler,
  bool is_parameter_path)
  : path(node_path),
    handler(node_handler),
    sub_nodes(),
    is_parameter(is_parameter_path) {
}

Routing::Routing()
  : root_(std::make_unique<Routing::RoutingNode>(kRootPath)) {
}

Routing::~Routing() = default;

void Routing::RegistHandler(
  const std::string& path,
  std::function<void()> handler) {
  if (0 == path.compare(kRootPath)) {
    root_->path = path;
    root_->handler = handler;
    return;
  }

  std::vector<std::string> splited_path;
  SplitPathBySlash(path, &splited_path);

  RegistHandlerInternal(&splited_path, &root_, handler);
}

std::function<void()> Routing::Route(const std::string& path) {
  if (0 == path.compare(kRootPath)) {
    return root_->handler;
  }

  std::vector<std::string> splited_path;
  SplitPathBySlash(path, &splited_path);

  Routing::RoutingNode* node = SearchRoutingNode(&splited_path, &root_);
  if (nullptr == node) {
    return {};
  }
  return node->handler;
}

void Routing::RegistHandlerInternal(
  std::vector<std::string>* splited_path,
  std::unique_ptr<Routing::RoutingNode>* current_node,
  std::function<void()> handler) {
  if (splited_path->empty()) {
    (*current_node)->handler = handler;
    return;
  }

  std::string current_path = *splited_path->begin();
  splited_path->erase(splited_path->begin());

  Routing::RoutingNode::SubRoutingNodeMap& sub_modules =
    (*current_node)->sub_nodes;
  if (sub_modules.find(current_path) == sub_modules.end()) {
    std::unique_ptr<Routing::RoutingNode> node =
      std::make_unique<Routing::RoutingNode>(current_path);
    sub_modules.insert({current_path, std::move(node)});
  }

  auto& node = sub_modules.at(current_path);
  RegistHandlerInternal(splited_path, &node, handler);
}

Routing::RoutingNode* Routing::SearchRoutingNode(
  std::vector<std::string>* splited_path,
  std::unique_ptr<Routing::RoutingNode>* current_node) {
  if (splited_path->empty()) {
    return (*current_node).get();
  }

  std::string current_path = *splited_path->begin();
  splited_path->erase(splited_path->begin());

  Routing::RoutingNode::SubRoutingNodeMap& sub_modules =
    (*current_node)->sub_nodes;
  if (sub_modules.find(current_path) == sub_modules.end()) {
    return nullptr;
  }

  auto& node = sub_modules.at(current_path);
  return SearchRoutingNode(splited_path, &node);
}

// std::unique_ptr<Routing::RoutingNode> Routing::CreateNewRoutingNode(
//   const std::string& path,
//   std::function<void()> handler) {
//   auto node = std::make_unique<Routing::RoutingNode>();
//   node->path = path;
//   node->handler = handler;
//   return std::move(node);
// }

}  // namespace http
}  // namespace net
}  // namespace nlink

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
  std::vector<Routing::SplitedPath>* splited_path) {
  if (origin.empty()) {
    return;
  }

  std::stringstream stream(origin);
  std::string item;
  while (std::getline(stream, item, kPathDelimiter)) {
    if (item.empty()) {
      continue;
    }

    bool is_parameter = false;
    char first = *item.begin();
    char last = *item.rbegin();

    if (('<' == first) && ('>' == last)) {
      is_parameter = true;
    }

    splited_path->emplace_back(item, is_parameter);
  }
}

Routing::RoutingNode::RoutingNode(
  const std::string& node_path,
  bool is_parameter_path,
  std::function<void()> node_handler)
  : path(node_path),
    is_parameter(is_parameter_path),
    handler(node_handler),
    sub_nodes() {
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

  std::vector<SplitedPath> splited_path;
  SplitPathBySlash(path, &splited_path);

  RegistHandlerInternal(&splited_path, &root_, handler);
}

std::function<void()> Routing::Route(const std::string& path) {
  if (0 == path.compare(kRootPath)) {
    return root_->handler;
  }

  std::vector<SplitedPath> splited_path;
  SplitPathBySlash(path, &splited_path);

  Routing::RoutingNode* node = SearchRoutingNode(&splited_path, &root_);
  if (nullptr == node) {
    return {};
  }
  return node->handler;
}

void Routing::RegistHandlerInternal(
  std::vector<Routing::SplitedPath>* splited_path,
  std::unique_ptr<Routing::RoutingNode>* current_node,
  std::function<void()> handler) {
  if (splited_path->empty()) {
    (*current_node)->handler = handler;
    return;
  }

  Routing::SplitedPath current_item = *splited_path->begin();
  std::string current_path = current_item.first;
  bool is_parameter_path = current_item.second;
  splited_path->erase(splited_path->begin());

  Routing::RoutingNode::SubRoutingNodeMap& sub_modules =
    (*current_node)->sub_nodes;

  std::unique_ptr<Routing::RoutingNode>* netx_node = nullptr;
  std::unique_ptr<Routing::RoutingNode>* parameter_path_node =
    FindParameterPathOnSubModules(sub_modules);
  if (nullptr != parameter_path_node) {
    netx_node = parameter_path_node;
  } else {
    if (sub_modules.find(current_path) == sub_modules.end()) {
      std::unique_ptr<Routing::RoutingNode> node =
        std::make_unique<Routing::RoutingNode>(current_path, is_parameter_path);
      sub_modules.insert({current_path, std::move(node)});
    }

    netx_node = &(sub_modules.at(current_path));
  }

  RegistHandlerInternal(splited_path, netx_node, handler);
}

Routing::RoutingNode* Routing::SearchRoutingNode(
  std::vector<Routing::SplitedPath>* splited_path,
  std::unique_ptr<Routing::RoutingNode>* current_node) {
  if (splited_path->empty()) {
    return (*current_node).get();
  }

  Routing::SplitedPath current_item = *splited_path->begin();
  std::string current_path = current_item.first;
  splited_path->erase(splited_path->begin());

  std::unique_ptr<Routing::RoutingNode>* netx_node = nullptr;
  Routing::RoutingNode::SubRoutingNodeMap& sub_modules =
    (*current_node)->sub_nodes;
  if (sub_modules.find(current_path) == sub_modules.end()) {
    std::unique_ptr<Routing::RoutingNode>* parameter_path_node =
      FindParameterPathOnSubModules(sub_modules);
    if (nullptr == parameter_path_node) {
      return nullptr;
    }
    netx_node = parameter_path_node;
  } else {
    netx_node = &(sub_modules.at(current_path));
  }

  return SearchRoutingNode(splited_path, netx_node);
}

std::unique_ptr<Routing::RoutingNode>* Routing::FindParameterPathOnSubModules(
  Routing::RoutingNode::SubRoutingNodeMap& sub_nodes) {
  for (auto& item : sub_nodes) {
    auto* node = &(item.second);
    bool is_parameter = item.second->is_parameter;
    if (is_parameter) {
      return node;
    }
  }
  return nullptr;
}

}  // namespace http
}  // namespace net
}  // namespace nlink

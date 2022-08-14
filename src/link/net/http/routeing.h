/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_HTTP_ROUTING_H_
#define LINK_NET_HTTP_ROUTING_H_

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>

#include "link/net/base/uri.h"
#include "link/net/http/method.h"
#include "link/net/http/status_code.h"
#include "link/net/http/version.h"
#include "link/net/http/header.h"

namespace nlink {
namespace net {
namespace http {

class Routing {
 public:
  struct RoutingNode {
    using SubRoutingNodeMap =
      std::map<std::string, std::unique_ptr<RoutingNode>>;

    RoutingNode(
      const std::string& node_path,
      std::function<void()> node_handler = {},
      bool is_parameter_path = false);

    std::string path;
    std::function<void()> handler;
    SubRoutingNodeMap sub_nodes;
    bool is_parameter;
  };

  Routing();
  ~Routing();

  void RegistHandler(
    const std::string& path,
    std::function<void()> handler);

  std::function<void()> Route(const std::string& path);
  

 private:
  void RegistHandlerInternal(
    std::vector<std::string>* splited_path,
    std::unique_ptr<RoutingNode>* current_node,
    std::function<void()> handler);

  RoutingNode* SearchRoutingNode(
    std::vector<std::string>* splited_path,
    std::unique_ptr<RoutingNode>* current_nodez);

  std::unique_ptr<RoutingNode> CreateNewRoutingNode(
    const std::string& path,
    std::function<void()> handler = {});

  std::unique_ptr<RoutingNode> root_;
};

void SplitPathBySlash(
  const std::string& origin,
  std::vector<std::string>* splited_path);

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_ROUTING_H_

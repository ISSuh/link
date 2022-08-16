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
#include <utility>
#include <functional>

#include "link/net/http/handler.h"

namespace nlink {
namespace net {
namespace http {

class Routing {
 public:
  using SplitedPath = std::pair<std::string, bool>;

  struct RoutingNode {
    using SubRoutingNodeMap =
      std::map<std::string, std::unique_ptr<RoutingNode>>;

    RoutingNode(
      const std::string& node_path,
      bool is_parameter_path = false,
      handler::ResponseHandler node_handler = {});

    std::string path;
    bool is_parameter;
    handler::ResponseHandler handler;
    SubRoutingNodeMap sub_nodes;
  };

  Routing();
  ~Routing();

  void RegistHandler(
    const std::string& path,
    handler::ResponseHandler handler);

  handler::ResponseHandler Route(const std::string& path) const;

 private:
  void RegistHandlerInternal(
    std::vector<SplitedPath>* splited_path,
    const std::unique_ptr<RoutingNode>& current_node,
    handler::ResponseHandler handler);

  RoutingNode* SearchRoutingNode(
    std::vector<SplitedPath>* splited_path,
    const std::unique_ptr<RoutingNode>& current_nodez) const;

  const std::unique_ptr<RoutingNode>* FindParameterPathOnSubModules(
    const RoutingNode::SubRoutingNodeMap& sub_nodes) const;

  std::unique_ptr<RoutingNode> root_;
};

void SplitPathBySlash(
  const std::string& origin,
  std::vector<Routing::SplitedPath>* splited_path);

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_ROUTING_H_

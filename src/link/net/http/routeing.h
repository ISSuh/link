/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_HTTP_ROUTING_H_
#define LINK_NET_HTTP_ROUTING_H_

#include <string>
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

    std::string path;
    std::function<void()> handler;
    SubRoutingNodeMap sub_nodes;
  };

  Routing();
  ~Routing();

  void RegistHandler(
    const std::string& path,
    std::function<void()> handler);

 private:
  void SearchPath(const std::string& path);
  void RecursiceSearch();

  void RecursiveInsert(
    const std::string& path,
    std::function<void()> handler,
    std::unique_ptr<RoutingNode>& current_node);

  std::unique_ptr<RoutingNode> CreateNewRoutingNode(
    const std::string& path,
    std::function<void()> handler);

  std::unique_ptr<RoutingNode> root_;
};

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_ROUTING_H_

/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_HTTP_HTTP_SERVER_COMPONENT_H_
#define LINK_COMPONENT_HTTP_HTTP_SERVER_COMPONENT_H_

#include <string>
#include <memory>

#include "link/base/macro.h"
#include "link/base/task/task_runner.h"
#include "link/net/http/handler.h"
#include "link/net/http/routeing.h"
#include "link/io/socket/server.h"
#include "link/component/ipc/socket/socket_component.h"
#include "link/component/http/http_component.h"
#include "link/component/http/server/http_server.h"

namespace nlink {
namespace component {

class HttpComponent;

class HttpServerComponent : public HttpComponent {
 public:
  static HttpServerComponent* CreateComponent(
    base::ComponentChannelController* channel_controller,
    std::weak_ptr<base::TaskRunner> task_runner);

  void Open(const std::string& address, int32_t port);
  void Close();

  void Route(
    const std::string& path,
    net::http::handler::ResponseHandler handler);

 private:
  explicit HttpServerComponent(
    base::ComponentChannelController* channel_controller,
    std::weak_ptr<base::TaskRunner> task_runner);
  virtual ~HttpServerComponent();

  std::weak_ptr<base::TaskRunner> task_runner_;
  std::unique_ptr<HttpServer> http_server_;

  DISAALOW_COPY_AND_ASSIGN(HttpServerComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_HTTP_HTTP_SERVER_COMPONENT_H_

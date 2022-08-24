/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_HTTP_SERVER_HTTP_SERVER_H_
#define LINK_COMPONENT_HTTP_SERVER_HTTP_SERVER_H_

#include <string>
#include <memory>

#include "link/base/macro.h"
#include "link/base/task/task_runner.h"
#include "link/io/socket/server.h"
#include "link/net/http/handler.h"
#include "link/net/http/routeing.h"

namespace nlink {
namespace component {

class HttpServer {
 public:
  explicit HttpServer(base::TaskRunner* task_runner);
  virtual ~HttpServer();

  io::Server* ServerContext() const;

  void Open(const io::IpEndPoint& endpoint);
  void Close();

  void Route(
    const std::string& path,
    net::http::handler::ResponseHandler handler);

 private:
  void InternalAcceptHandler(std::shared_ptr<io::Session> session);
  void InternalCloseHandler(std::shared_ptr<io::Session> session);
  void InternalReadHandler(
    const base::Buffer& buffer, std::shared_ptr<io::Session> session);
  void InternalWriteHandler(size_t length);

  base::TaskRunner* task_runner_;
  std::unique_ptr<io::Server> io_server_;

  net::http::Routing routing_;

  DISAALOW_COPY_AND_ASSIGN(HttpServer);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_HTTP_SERVER_HTTP_SERVER_H_

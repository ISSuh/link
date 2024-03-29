/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/http/server/http_server.h"

#include "link/base/logging.h"
#include "link/io/socket/socket_factory.h"
#include "link/net/http/parser.h"
#include "link/net/http/request.h"
#include "link/net/http/response.h"
#include "link/component/http/server/http_util.h"

namespace nlink {
namespace component {

HttpServer::HttpServer(std::weak_ptr<base::TaskRunner> task_runner)
  : task_runner_(task_runner),
    io_server_(io::SocketFactory::CreateTcpServer(task_runner)) {
}

HttpServer::~HttpServer() = default;

io::Server* HttpServer::ServerContext() const {
  return io_server_.get();
}

void HttpServer::Open(const io::IpEndPoint& endpoint) {
  io_server_->RegistIOHandler(
    [this](const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
      this->InternalReadHandler(buffer, session);
    },
    [this](size_t length) {
      this->InternalWriteHandler(length);
    });

  io_server_->Listen(endpoint,
    [this](std::shared_ptr<io::Session> session) {
      this->InternalAcceptHandler(session);
    },
    [this](std::shared_ptr<io::Session> session) {
      this->InternalCloseHandler(session);
    });
}

void HttpServer::Close() {
  io_server_->Close();
}

void HttpServer::Route(
  const std::string& path,
  net::http::handler::ResponseHandler handler) {
  routing_.RegistHandler(path, handler);
}

void HttpServer::InternalAcceptHandler(std::shared_ptr<io::Session>) {
}

void HttpServer::InternalCloseHandler(std::shared_ptr<io::Session>) {
}

void HttpServer::InternalReadHandler(
  const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
  LOG(INFO) << __func__ << " - buffer size : " << buffer.Size();
  if (buffer.IsEmpty()) {
    return;
  }

  net::http::Request request = net::http::Parser::ParseRequest(buffer);
  if (!request.IsValid()) {
    return;
  }

  auto url  = request.RequestUri();
  if (!url.HasPath()) {
    return;
  }

  net::http::Response::StatusLine status_line(
    net::http::HttpStatusCode::OK,
    url.Path(),
    request.HttpVersion());

  net::http::Response response(status_line);

  auto handler = routing_.Route(url.Path());
  if (!handler) {
    response = http::Default404Error(url.Path());
  } else {
    handler(request, &response);
  }

  std::shared_ptr<base::Buffer> response_buffer =
    std::make_shared<base::Buffer>(response.Serialize());
  session->Write(response_buffer);
}

void HttpServer::InternalWriteHandler(size_t) {
}

}  // namespace component
}  // namespace nlink


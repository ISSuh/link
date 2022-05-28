/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/socket/asio/connector.h"

#include <string>
#include <memory>
#include <utility>

#include "link/net/socket/asio/tcp_session.h"
#include "link/third_party/asio/asio/io_context.hpp"
#include "link/third_party/asio/asio/ip/tcp.hpp"

namespace nlink {
namespace net {

class ConnectorImpl : public Connector {
 public:
  explicit ConnectorImpl(asio::io_context* io_context);
  virtual ~ConnectorImpl() = default;

  void Connect(
    const IpEndPoint& address, Connector::OnConnect handler) override;

 private:
  void InternalConnectHnadler(const asio::error_code& error);

  asio::ip::tcp::socket socket_;
  Connector::OnConnect on_connect_;
};

ConnectorImpl::ConnectorImpl(asio::io_context* io_context)
  : socket_(*io_context) {
}

void ConnectorImpl::Connect(
  const IpEndPoint& address, Connector::OnConnect handler) {
  on_connect_ = std::move(handler);

  const std::string address_str = address.address().ToString();
  int32_t port = address.port();

  asio::ip::tcp::endpoint endpoint(
    asio::ip::address::from_string(address_str), port);
  socket_.async_connect(endpoint,
    std::bind(&ConnectorImpl::InternalConnectHnadler, this,
      std::placeholders::_1));
}

void ConnectorImpl::InternalConnectHnadler(const asio::error_code& error) {
  std::shared_ptr<Session> session = nullptr;

  if (!error) {
    session = std::make_shared<TcpSession>(std::move(socket_));
  }
  on_connect_.Run(session);
}

Connector* Connector::CreateConnector(
  base::DispatcherConext* dispatcher_context) {
  void* context_ptr = dispatcher_context->context();
  if (nullptr == context_ptr) {
    return nullptr;
  }

  asio::io_context* io_context = static_cast<asio::io_context*>(context_ptr);
  return new ConnectorImpl(io_context);
}

}  // namespace net
}  // namespace nlink

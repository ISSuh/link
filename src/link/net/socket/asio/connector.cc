/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/socket/asio/connector.h"

#include <string>
#include <memory>
#include <utility>

#include "link/third_party/asio/asio/io_context.hpp"
#include "link/third_party/asio/asio/ip/tcp.hpp"

namespace nlink {
namespace net {

class ConnectorImpl;

Connector* Connector::CreateConnector(
  base::DispatcherConext* dispatcher_context) {
  void* context_ptr = dispatcher_context->context();
  if (nullptr == context_ptr) {
    return nullptr;
  }

  asio::io_context* io_context = static_cast<asio::io_context*>(context_ptr);
  return new ConnectorImpl(io_context);
}

class ConnectorImpl : public Connector {
 public:
  explicit ConnectorImpl(asio::io_context* io_context);
  virtual ~ConnectorImpl() = default;

  void Connect(
    const IpEndPoint& address, Connector::ConnectHandler handler) override;

 private:
  void InternalConnectHnadler(const asio::error_code& error);

  asio::ip::tcp::socket socket_;
  Connector::ConnectHandler connect_handler_;
};

ConnectorImpl::ConnectorImpl(asio::io_context* io_context)
  : socket_(*io_context) {
}

void ConnectorImpl::Connect(
  const IpEndPoint& address, Connector::ConnectHandler handler) {
  connect_handler_ = handler;

  const std::string address_str = address.address().ToString();
  int32_t port = address.port();

  asio::ip::tcp::endpoint endpoint(
    asio::ip::address::from_string(address_str), port);
  socket_.async_connect(endpoint, InternalConnectHnadler);
}

void ConnectorImpl::InternalConnectHnadler(const asio::error_code& error) {
  std::shared_ptr<ClientSideSession> session = nullptr;

  if (!error) {
    session = std::make_shared<ClientSideSession>(std::move(socket_));
  }
  connect_handler_.Run(session);
}

}  // namespace net
}  // namespace nlink

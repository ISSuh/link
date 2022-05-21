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

Connector* Connector::CreateConnector(asio::ip::tcp::socket socket) {
  return new ConnectorImpl(std::move(socket));
}

class ConnectorImpl : public Connector {
 public:
  explicit ConnectorImpl(asio::ip::tcp::socket socket);
  virtual ~ConnectorImpl() = default;

  void Connect(
    const IpEndPoint& address, Session::Delegate* delegate) override;
  void Close() override;

 private:
  void ConnectHnadler(const asio::error_code& error);

  asio::ip::tcp::socket socket_;
  Session::Delegate* delegate_;
};

ConnectorImpl::ConnectorImpl(asio::ip::tcp::socket socket)
  : socket_(std::move(socket)), delegate_(nullptr) {
}

void ConnectorImpl::Connect(
  const IpEndPoint& address, Session::Delegate* delegate) {
  delegate_ = delegate;

  const std::string address_str = address.address().ToString();
  int32_t port = address.port();

  asio::ip::tcp::endpoint endpoint(
    asio::ip::address::from_string(address_str), port);
  socket_.async_connect(endpoint, ConnectHnadler);
}

void ConnectorImpl::ConnectHnadler(const asio::error_code& error) {
  if (!error) {
    // todo
  }
}

}  // namespace net
}  // namespace nlink

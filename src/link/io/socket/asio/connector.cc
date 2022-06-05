/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/socket/asio/connector.h"

#include <string>
#include <memory>
#include <utility>
#include <chrono>

#include "link/base/logging.h"
#include "link/io/socket/asio/tcp_session.h"
#include "link/third_party/asio/asio/io_context.hpp"
#include "link/third_party/asio/asio/ip/tcp.hpp"
#include "link/third_party/asio/asio/steady_timer.hpp"

namespace nlink {
namespace io {

const int32_t kExpireTime = 10;

class ConnectorImpl : public Connector {
 public:
  explicit ConnectorImpl(asio::io_context* io_context);
  virtual ~ConnectorImpl();

  void Connect(
    const IpEndPoint& address, handler::ConnectHandler handler) override;

 private:
  void InternalConnectHnadler(const asio::error_code& error);
  void CheckingExpired();

  asio::io_context* io_context_;
  std::unique_ptr<asio::ip::tcp::socket> socket_;

  IpEndPoint address_;
  handler::ConnectHandler connect_handler_;

  asio::steady_timer expired_timer_;
  uint64_t try_connection_count_;
};

ConnectorImpl::ConnectorImpl(asio::io_context* io_context)
  : io_context_(io_context),
    socket_(nullptr),
    expired_timer_(*io_context),
    try_connection_count_(0) {
}

ConnectorImpl::~ConnectorImpl() {
  expired_timer_.cancel();
}

void ConnectorImpl::Connect(
  const IpEndPoint& address, handler::ConnectHandler handler) {
  LOG(INFO) << "[ConnectorImpl::Connect]";
  address_ = address;

  if (connect_handler_.is_null()) {
    connect_handler_ = std::move(handler);
  }

  if (nullptr == socket_) {
    socket_.reset(new asio::ip::tcp::socket(*io_context_));
  }

  const std::string address_str = address.address().ToString();
  int32_t port = address.port();

  asio::ip::tcp::endpoint endpoint(
    asio::ip::address::from_string(address_str), port);

  socket_->async_connect(endpoint,
    std::bind(&ConnectorImpl::InternalConnectHnadler, this,
      std::placeholders::_1));

  expired_timer_.expires_after(std::chrono::seconds(kExpireTime));
  expired_timer_.async_wait(
    std::bind(&ConnectorImpl::CheckingExpired, this));
}

void ConnectorImpl::InternalConnectHnadler(const asio::error_code& error) {
  LOG(INFO) << "[ConnectorImpl::InternalConnectHnadler]";
  std::shared_ptr<Session> session = nullptr;
  if (nullptr != socket_ && !socket_->is_open()) {
    LOG(WARNING) << "[ConnectorImpl::InternalConnectHnadler]"
              << " timeout";
    Connect(address_, connect_handler_);
  } else if (error) {
    LOG(WARNING) << "[ConnectorImpl::InternalConnectHnadler]"
              << " connect error : " << error.message();

  if (nullptr != socket_) {
    socket_.reset();
  }

    Connect(address_, connect_handler_);
  } else {
    expired_timer_.expires_at(asio::steady_timer::time_point::max());

    session = std::make_shared<TcpSession>(std::move(*socket_));
    connect_handler_.Run(session);

    socket_.reset();
  }
}

void ConnectorImpl::CheckingExpired() {
  LOG(INFO) << "[ConnectorImpl::CheckingExpired]"
            << " try_connection_count : " << try_connection_count_;

  if (nullptr == socket_) {
    return;
  }

  if (expired_timer_.expiry() <= asio::steady_timer::clock_type::now()) {
    socket_->cancel();
    socket_->close();
    expired_timer_.expires_at(asio::steady_timer::time_point::max());
  }

  ++try_connection_count_;
  Connect(address_, connect_handler_);
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

}  // namespace io
}  // namespace nlink

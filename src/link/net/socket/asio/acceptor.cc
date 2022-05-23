/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */
#include "link/net/socket/asio/acceptor.h"

#include <string>
#include <memory>
#include <utility>
#include <functional>

#include "link/base/logging.h"
#include "link/net/socket/asio/session_manager.h"
#include "link/third_party/asio/asio/io_context.hpp"
#include "link/third_party/asio/asio/ip/tcp.hpp"

namespace nlink {
namespace net {

class AcceptorImpl : public Acceptor {
 public:
  explicit AcceptorImpl(base::DispatcherConext* context);
  virtual ~AcceptorImpl() = default;

  void Listen(const IpEndPoint& address) override;
  void Accept(Acceptor::AcceptorHandler callback) override;
  void Close() override;

 private:
  void DoAccept();
  void AcceptHandler(std::error_code ec, asio::ip::tcp::socket socket);

  base::DispatcherConext* dispatcher_contex_;
  std::unique_ptr<asio::ip::tcp::acceptor> acceptor_;

  Acceptor::AcceptorHandler accept_callback_;
};

AcceptorImpl::AcceptorImpl(base::DispatcherConext* dispatcher_context)
  : dispatcher_contex_(dispatcher_context),
    acceptor_(nullptr) {
}

void AcceptorImpl::Listen(const IpEndPoint& address) {
  void* context = dispatcher_contex_->context();
  asio::io_context* io_context = static_cast<asio::io_context*>(context);

  acceptor_.reset(new asio::ip::tcp::acceptor(*io_context));
  asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), address.port());

  acceptor_->open(endpoint.protocol());
  acceptor_->set_option(asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_->bind(endpoint);
  acceptor_->listen();
}

void AcceptorImpl::Accept(AcceptorHandler callback) {
  accept_callback_ = std::move(callback);
  DoAccept();
}

void AcceptorImpl::DoAccept() {
  acceptor_->async_accept(
    std::bind(
      &AcceptorImpl::AcceptHandler, this,
      std::placeholders::_1, std::placeholders::_2));
}

void AcceptorImpl::AcceptHandler(
  std::error_code ec, asio::ip::tcp::socket socket) {
  LOG(INFO) << __func__;
  if (!acceptor_->is_open()) {
    return;
  }

  if (!ec) {
    std::shared_ptr<Session> session =
      std::make_shared<Session>(std::move(socket));
    accept_callback_.Run(std::move(session));
  }

  DoAccept();
}

void AcceptorImpl::Close() {
  acceptor_->close();
}

Acceptor* Acceptor::CreateAcceptor(
  base::DispatcherConext* dispatcher_context) {
  return new AcceptorImpl(dispatcher_context);
}

}  // namespace net
}  // namespace nlink

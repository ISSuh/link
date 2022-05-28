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
#include "link/net/socket/asio/tcp_session.h"
#include "link/third_party/asio/asio/io_context.hpp"
#include "link/third_party/asio/asio/ip/tcp.hpp"

namespace nlink {
namespace net {

class AcceptorImpl : public Acceptor {
 public:
  explicit AcceptorImpl(asio::io_context* io_context);
  virtual ~AcceptorImpl() = default;

  bool Listen(const IpEndPoint& address) override;
  void Accept(Acceptor::OnAccept callback) override;
  void Close() override;

 private:
  void DoAccept();
  void InternalAcceptHandler(std::error_code ec, asio::ip::tcp::socket socket);

  asio::ip::tcp::acceptor acceptor_;
  Acceptor::OnAccept on_accept_;
};

AcceptorImpl::AcceptorImpl(asio::io_context* io_context)
  : acceptor_(*io_context) {
}

bool AcceptorImpl::Listen(const IpEndPoint& address) {
  asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), address.port());

  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();
  return acceptor_.is_open();
}

void AcceptorImpl::Accept(Acceptor::OnAccept callback) {
  on_accept_ = std::move(callback);
  DoAccept();
}

void AcceptorImpl::DoAccept() {
  acceptor_.async_accept(
    std::bind(
      &AcceptorImpl::InternalAcceptHandler, this,
      std::placeholders::_1, std::placeholders::_2));
}

void AcceptorImpl::InternalAcceptHandler(
  std::error_code ec, asio::ip::tcp::socket socket) {
  std::shared_ptr<TcpSession> session = nullptr;

  if (!acceptor_.is_open()) {
    on_accept_.Run(session);
    return;
  }


  if (!ec) {
    session = std::make_shared<TcpSession>(std::move(socket));
  }

  on_accept_.Run(session);
  DoAccept();
}

void AcceptorImpl::Close() {
  acceptor_.close();
}

Acceptor* Acceptor::CreateAcceptor(
  base::DispatcherConext* dispatcher_context) {
  void* context_ptr = dispatcher_context->context();
  if (nullptr == context_ptr) {
    return nullptr;
  }

  asio::io_context* io_context = static_cast<asio::io_context*>(context_ptr);
  return new AcceptorImpl(io_context);
}

}  // namespace net
}  // namespace nlink

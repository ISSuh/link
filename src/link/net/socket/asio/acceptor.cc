/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */
#include "link/net/socket/asio/acceptor.h"

#include <string>
#include <memory>
#include <utility>

#include "link/third_party/asio/asio/io_context.hpp"
#include "link/third_party/asio/asio/ip/tcp.hpp"

namespace nlink {
namespace net {

class AcceptorImpl;

std::unique_ptr<Acceptor> CreateAcceptor(
  base::DispatcherConext* dispatcher_context) {
  asio::io_context* context =
    static_cast<asio::io_context*>(dispatcher_context->context().get());

  return std::unique_ptr<Acceptor>(new AcceptorImpl(context));
}

class AcceptorImpl : public Acceptor {
 public:
  explicit AcceptorImpl(asio::io_context* io_context);

  void Listen(const IpEndPoint& address) override;
  void Accept(base::CompletionCallback callback) override;

 private:
  void DoAccept();

  asio::io_context* io_context_;
  std::unique_ptr<asio::ip::tcp::acceptor> acceptor_;

  base::CompletionCallback accept_callback_;
};

AcceptorImpl::AcceptorImpl(asio::io_context* io_context)
  : io_context_(io_context), acceptor_(nullptr) {
}

void AcceptorImpl::Listen(const IpEndPoint& address) {
    acceptor_.reset(new asio::ip::tcp::acceptor(*io_context_));
    asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), address.port());

    acceptor_->open(endpoint.protocol());
    acceptor_->set_option(asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_->bind(endpoint);
    acceptor_->listen();
}

void AcceptorImpl::Accept(base::CompletionCallback callback) {
  accept_callback_ = std::move(callback);
  DoAccept();
}

void AcceptorImpl::DoAccept() {
  acceptor_->async_accept(
    [this](std::error_code ec, asio::ip::tcp::socket socket) {
      if (!acceptor_->is_open()) {
        return;
      }

      if (!ec) {
        // call callback
      }

      DoAccept();
    });
}

}  // namespace net
}  // namespace nlink

/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */
#include "link/net/socket/asio/acceptor.h"

#include <string>
#include <memory>
#include <utility>

#include "link/net/socket/asio/session_manager.h"
#include "link/third_party/asio/asio/io_context.hpp"
#include "link/third_party/asio/asio/ip/tcp.hpp"

namespace nlink {
namespace net {

class AcceptorImpl;

class AcceptorImpl : public Acceptor {
 public:
  explicit AcceptorImpl(base::DispatcherConext* context);
  virtual ~AcceptorImpl() = default;

  void Listen(const IpEndPoint& address) override;
  void Accept(base::CompletionCallback callback) override;
  void Close() override;

 private:
  void DoAccept();

  SessionManager session_manager_;
  base::DispatcherConext* dispatcher_contex_;
  std::unique_ptr<asio::ip::tcp::acceptor> acceptor_;

  base::CompletionCallback accept_callback_;
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
        session_manager_.CreateSession(std::move(socket));
        accept_callback_.Run(1);
        // call callback
      }

      DoAccept();
    });
}

void AcceptorImpl::Close() {
  acceptor_->close();
}

Acceptor* Acceptor::CreateAcceptor(base::DispatcherConext* dispatcher_context) {
  return new AcceptorImpl(dispatcher_context);
}

}  // namespace net
}  // namespace nlink

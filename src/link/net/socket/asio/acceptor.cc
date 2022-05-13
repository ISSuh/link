/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */
#include "link/net/socket/asio/acceptor.h"

#include <string>

#include "link/third_party/asio/asio/ip/tcp.hpp"

namespace link {
namespace net {

class AcceptorImpl : public Acceptor {
 public:
  void Listen(const IpEndPoint& address) override;
  void Accept(base::CompletionCallback callback) override;

 private:
  asio::io_context& io_context_;
  asio::ip::tcp::acceptor acceptor_;

  base::CompletionCallback accept_callback_;
};

void AcceptorImpl::Listen(const IpEndPoint& address) {
    const std::string addr = address.address().ToString();
    const std::string port = std::to_string(address.port());

    asio::ip::tcp::resolver resolver(io_context_);
    asio::ip::tcp::endpoint endpoint =
      *resolver.resolve(address, port).begin();

    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();
}

void AcceptorImpl::Accept(base::CompletionCallback callback) {
  accept_callback_ = std::move(callback);

  acceptor_.async_accept(
    [this](std::error_code ec, asio::ip::tcp::socket socket) {
      if (!acceptor_.is_open()) {
        return;
      }

      if (!ec) {
        accept_callback_.Run():
      }

      do_accept();
    });
}

}  // namespace net
}  // namespace link

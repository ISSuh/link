/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/socket/asio/tcp_client.h"

#include "link/base/event/event_dispatcher.h"
#include "link/third_party/asio/asio/io_context.hpp"
#include "link/third_party/asio/asio/ip/tcp.hpp"

namespace nlink {
namespace net {

TcpClient::TcpClient()
  : session_(nullptr) {
}

TcpClient::~TcpClient() {
}

void TcpClient::Connect(IpEndPoint address) {
  asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), address.port());
}

void TcpClient::DisConnect() {
}

void TcpClient::Write() {
}

void TcpClient::OpenChannel(base::DispatcherConext* context) {
  if (!context) {
    return;
  }

  void* context_ptr = context->context();
  asio::io_context* io_context = static_cast<asio::io_context*>(context_ptr);

  asio::ip::tcp::socket socket(io_context);
  session_.reset(new Session(std::move(socket), this));
}

void TcpClient::CloseChannel() {
  session_->Close();
}

void TcpClient::HandleEvent(const base::Event& event) {
}

}  // namespace net
}  // namespace nlink

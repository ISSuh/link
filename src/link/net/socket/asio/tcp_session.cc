/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/socket/asio/tcp_session.h"

#include <utility>

#include "link/base/logging.h"
#include "link/third_party/asio/asio/read.hpp"

namespace nlink {
namespace net {

const int32_t kMaxPacketSize = 8192;

TcpSession::TcpSession(asio::ip::tcp::socket socket)
  : socket_(std::move(socket)),
    read_buffer_(kMaxPacketSize) {
}

TcpSession::~TcpSession() {}

void TcpSession::Open(
  handler::ReadHandler read_handler,
  handler::WriteHandler write_handler,
  handler::CloseHandler close_handler) {
  read_handler_ = read_handler;
  write_handler_ = write_handler;
  close_handler_ = close_handler;

  DoRead();
}

void TcpSession::Close() {
  LOG(INFO) << "[TcpSession::Close]";
  socket_.close();
  close_handler_.Run(shared_from_this());
}

void TcpSession::Write(const base::Buffer& buffer) {
  socket_.async_write_some(asio::buffer(buffer.Data()),
    std::bind(&TcpSession::InternalWriteHandler, this,
      std::placeholders::_1, std::placeholders::_2));
}

bool TcpSession::IsConnected() const {
  return socket_.is_open();
}

void TcpSession::InternalWriteHandler(
  std::error_code ec, std::size_t length) {
  if (ec) {
    return;
  }

  write_handler_.Run(length);
}

void TcpSession::DoRead() {
  std::vector<uint8_t> buffer(kMaxPacketSize);

  socket_.async_read_some(asio::buffer(buffer),
    std::bind(&TcpSession::InternalReadHandler, this,
      std::move(buffer), std::placeholders::_1, std::placeholders::_2));
}

void TcpSession::InternalReadHandler(
  const std::vector<uint8_t>& buffer,
  const std::error_code& ec, std::size_t length) {
  if ((asio::error::eof == ec) || (asio::error::connection_reset == ec)) {
    close_handler_.Run(shared_from_this());
  } else {
    read_handler_.Run(base::Buffer(buffer.data(), length), shared_from_this());
    DoRead();
  }
}

}  // namespace net
}  // namespace nlink

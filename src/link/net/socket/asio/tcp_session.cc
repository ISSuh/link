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
  socket_.close();

  close_handler_.Run(shared_from_this());
}

void TcpSession::Write(const base::Buffer& buffer) {
  LOG(INFO) << "[TcpSession::Write]";
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

  asio::async_read(socket_, asio::buffer(buffer),
    std::bind(&TcpSession::InternalReadHandler, this,
      buffer, std::placeholders::_1, std::placeholders::_2));
}

void TcpSession::InternalReadHandler(
  const std::vector<uint8_t>& buffer, std::error_code ec, std::size_t length) {
  read_handler_.Run(base::Buffer(buffer));
  DoRead();
}

}  // namespace net
}  // namespace nlink

/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/socket/asio/tcp_session.h"

#include <utility>

#include "link/base/logging.h"
#include "link/third_party/asio/asio/read.hpp"

namespace nlink {
namespace io {

const int32_t kDefaultMaxPacketSize = 8192;

TcpSession::TcpSession(asio::ip::tcp::socket socket)
  : socket_(std::move(socket)),
    max_packet_size_(kDefaultMaxPacketSize),
    raw_read_buffer(max_packet_size_) {
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
  close_handler_.Run(shared_from_this());
  socket_.close();
}

void TcpSession::Write(const base::Buffer& buffer) {
  socket_.async_write_some(asio::buffer(buffer.Data()),
    std::bind(&TcpSession::InternalWriteHandler, shared_from_this(),
      handler::WriteHandler(), std::placeholders::_1, std::placeholders::_2));
}

void TcpSession::Write(
  const base::Buffer& buffer,
  handler::WriteHandler write_handler,
  handler::ReadHandler read_handler) {
  socket_.async_write_some(asio::buffer(buffer.Data()),
    std::bind(&TcpSession::InternalWriteHandler, shared_from_this(),
      write_handler, std::placeholders::_1, std::placeholders::_2));
}

bool TcpSession::IsConnected() const {
  return socket_.is_open();
}

void TcpSession::InternalWriteHandler(
  handler::WriteHandler write_handler,
  std::error_code ec,
  std::size_t length) {
  if (ec) {
    return;
  }

  if (!write_handler_.is_null()) {
    write_handler_.Run(length);
  }
}

void TcpSession::DoRead() {
  socket_.async_read_some(asio::buffer(raw_read_buffer),
    std::bind(&TcpSession::InternalReadHandler, shared_from_this(),
      std::placeholders::_1, std::placeholders::_2));
}

void TcpSession::InternalReadHandler(
  const std::error_code& ec, std::size_t length) {
  if ((asio::error::eof == ec) || (asio::error::connection_reset == ec)) {
    Close();
  } else {
    base::Buffer buff(raw_read_buffer);
    if (!read_handler_.is_null()) {
      read_handler_.Run(std::move(buff), shared_from_this());
    }
    DoRead();
  }
}

}  // namespace io
}  // namespace nlink

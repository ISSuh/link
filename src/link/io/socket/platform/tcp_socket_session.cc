/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/socket/platform/tcp_socket_ession.h"

#include <utility>

#include "link/base/logging.h"
#include "link/third_party/asio/asio/read.hpp"

namespace nlink {
namespace io {

const int32_t kDefaultMaxPacketSize = 8192;

TcpSocketSession::TcpSocketSession()
  : max_packet_size_(kDefaultMaxPacketSize),
    raw_read_buffer(max_packet_size_) {
}

TcpSocketSession::~TcpSocketSession() {}

void TcpSocketSession::Open(
  handler::ReadHandler read_handler,
  handler::WriteHandler write_handler,
  handler::CloseHandler close_handler) {
  read_handler_ = read_handler;
  write_handler_ = write_handler;
  close_handler_ = close_handler;

  DoRead();
}

void TcpSocketSession::Close() {
  LOG(INFO) << "[TcpSocketSession::Close]";
  close_handler_.Run(shared_from_this());
  socket_->close();
}

void TcpSocketSession::Write(const base::Buffer& buffer) {

}

void TcpSocketSession::Write(
  const base::Buffer& buffer,
  handler::WriteHandler write_handler,
  handler::ReadHandler read_handler) {

}

bool TcpSocketSession::IsConnected() const {
  return socket_->IsConnected();
}

// void TcpSocketSession::InternalWriteHandler(
//   handler::WriteHandler write_handler,
//   std::error_code ec,
//   std::size_t length) {
//   if (ec) {
//     return;
//   }

//   if (!write_handler_.is_null()) {
//     write_handler_.Run(length);
//   }
// }

void TcpSocketSession::DoRead() {
  socket_->async_read_some(asio::buffer(raw_read_buffer),
    std::bind(&TcpSocketSession::InternalReadHandler, shared_from_this(),
      std::placeholders::_1, std::placeholders::_2));
}

// void TcpSocketSession::InternalReadHandler(
//   const std::error_code& ec, std::size_t length) {
//   if ((asio::error::eof == ec) || (asio::error::connection_reset == ec)) {
//     Close();
//   } else {
//     base::Buffer buff(raw_read_buffer);
//     if (!read_handler_.is_null()) {
//       read_handler_.Run(std::move(buff), shared_from_this());
//     }
//     DoRead();
//   }
// }

}  // namespace io
}  // namespace nlink

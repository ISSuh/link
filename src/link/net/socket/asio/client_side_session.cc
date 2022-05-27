/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/socket/asio/client_side_session.h"

#include <utility>

#include "link/base/logging.h"
#include "link/third_party/asio/asio/read.hpp"

namespace nlink {
namespace net {

const int32_t kMaxPacketSize = 8192;

ClientSideSession::ClientSideSession(asio::ip::tcp::socket socket)
  : socket_(std::move(socket)),
    read_buffer_(kMaxPacketSize) {
}

ClientSideSession::~ClientSideSession() {
  if (socket_.is_open()) {
    Close();
  }
}

void ClientSideSession::Open(
  handler::ReadHandler read_handler,
  handler::WriteHandler write_handler,
  handler::CloseHandler close_handler) {
  read_handler_ = read_handler;
  write_handler_ = write_handler;
  close_handler_ = close_handler;
}

void ClientSideSession::Close() {
  socket_.close();

  close_handler_.Run(this);
}

void ClientSideSession::Write(const base::Buffer& buffer) {
  socket_.async_write_some(asio::buffer(buffer.Data()),
    std::bind(&ClientSideSession::InternalWriteHandler, this,
      std::placeholders::_1, std::placeholders::_2));
}

bool ClientSideSession::IsConnected() const {
  return socket_.is_open();
}

void ClientSideSession::InternalWriteHandler(
  std::error_code ec, std::size_t length) {
  if (ec) {
    return;
  }

  write_handler_.Run(length);
}

void ClientSideSession::DoRead() {
  std::vector<uint8_t> buffer(kMaxPacketSize);

  asio::async_read(socket_, asio::buffer(buffer),
    std::bind(&ClientSideSession::InternalReadHandler, this,
      std::placeholders::_1, std::placeholders::_2));
}

void ClientSideSession::InternalReadHandler(
  sstd::error_code ec, std::size_t length) {
  std::vector<uint8_t> temp(
    read_buffer_.begin(), read_buffer_.begin() + length);
  read_callback_.Run(std::move(temp));
  DoRead();
}

}  // namespace net
}  // namespace nlink

/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/socket/asio/session.h"

#include <utility>

#include "link/base/logging.h"

namespace nlink {
namespace net {

const int32_t kMaxPacketSize = 8192;

Session::Session(asio::ip::tcp::socket socket)
  : socket_(std::move(socket)),
    // delegate_(delegate),
    read_buffer_(kMaxPacketSize) {
}

Session::~Session() {}

void Session::Open(Server::ReadHandler read_handler) {
  read_callback_ = read_handler;
  DoRead();
}

void Session::Close() {
  socket_.close();
}

void Session::Connect(const std::string& address, int32_t port) {
  LOG(INFO) << __func__ << " - " << address << ":" << port;

  asio::ip::tcp::endpoint endpoint(
    asio::ip::address::from_string(address), port);
  socket_.async_connect(endpoint,
    std::bind(&Session::ConnectHandler, this, std::placeholders::_1));
}

void Session::Disconnect() {
  socket_.close();
}

void Session::ConnectHandler(const asio::error_code& error) {
  LOG(INFO) << __func__;
  
  if (!error) {
    DoRead();
  }
}

void Session::Write(
  const std::vector<uint8_t>& buffer, WriteCallback write_callback) {
  DoWrite(buffer, std::move(write_callback));
}

void Session::DoWrite(
  const std::vector<uint8_t>& buffer, WriteCallback write_callback) {
  socket_.async_write_some(asio::buffer(buffer),
    std::bind(&Session::WriteHandler, this,
      std::move(write_callback), std::placeholders::_1, std::placeholders::_2));
}

void Session::WriteHandler(
  WriteCallback write_callback, std::error_code ec, std::size_t length) {
  if (ec) {
    return;
  }

  write_callback.Run(length);
}

void Session::DoRead() {
  socket_.async_read_some(asio::buffer(read_buffer_),
    std::bind(&Session::ReadHandler, this,
      std::placeholders::_1, std::placeholders::_2));
}

void Session::ReadHandler(std::error_code ec, std::size_t length) {
  std::vector<uint8_t> temp(
    read_buffer_.begin(), read_buffer_.begin() + length);
  read_callback_.Run(std::move(temp));
  DoRead();
}


}  // namespace net
}  // namespace nlink

/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/socket/platform/tcp_session.h"

#include <utility>

#include "link/base/logging.h"

namespace nlink {
namespace io {

constexpr const size_t kDefaultBufferSize = 4096;

TcpSocketSession::TcpSocketSession(
  std::unique_ptr<TcpSocket> socket)
  : socket_(std::move(socket)),
    read_buffer_(kDefaultBufferSize) {
}

TcpSocketSession::~TcpSocketSession() {}

void TcpSocketSession::Open(
  handler::ReadHandler read_handler,
  handler::WriteHandler write_handler,
  handler::CloseHandler close_handler) {
  read_handler_ = read_handler;
  write_handler_ = write_handler;
  close_handler_ = close_handler;
}

void TcpSocketSession::Close() {
  LOG(INFO) << "[TcpSocketSession::Close]";
  close_handler_(shared_from_this());
  socket_->Close();
}

void TcpSocketSession::Read(base::Buffer* buffer) {
  int32_t res = socket_->Read(&read_buffer_,
    [this](int32_t res) {
      this->InternalReadHandler(read_buffer_, res);
    });

  InternalReadHandler(read_buffer_, res);
}

void TcpSocketSession::Write(const base::Buffer& buffer) {
  base::Buffer temp =  buffer;
  int32_t res = socket_->Write(&temp,
    [this](int32_t error_code ) {
      this->InternalWriteHandler(error_code);
    });

  InternalWriteHandler(res);
}

void TcpSocketSession::Write(
  const base::Buffer& buffer,
  handler::WriteHandler write_handler,
  handler::ReadHandler read_handler) {

}

bool TcpSocketSession::IsConnected() const {
  if (nullptr == socket_) {
    return false;
  }
  return socket_->IsConnected();
}

void TcpSocketSession::InternalWriteHandler(int32_t res) {
  if (res > 0) {
    if (!write_handler_) {
      return;
    }
    write_handler_(res);
  }
}

void TcpSocketSession::InternalReadHandler(
  const base::Buffer& buffer, int32_t res) {
  if (res > 0) {
    if (!read_handler_) {
      return;
    }
    read_handler_(buffer, shared_from_this());
  }
}

}  // namespace io
}  // namespace nlink

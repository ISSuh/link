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

TcpSocketSession::TcpSocketSession(
  base::TaskRunner* task_runner,
  std::shared_ptr<TcpSocket> socket)
  : task_runner_(task_runner), socket_(socket) {
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

void TcpSocketSession::Write(const base::Buffer& buffer) {
  LOG(INFO) << __func__;
  base::Buffer temp =  buffer;
  socket_->Write(&temp,
    [this](int32_t error_code ) {
      this->InternalWriteHandler(error_code);
    });
}

void TcpSocketSession::Write(
  const base::Buffer& buffer,
  handler::WriteHandler write_handler,
  handler::ReadHandler read_handler) {

}

bool TcpSocketSession::IsConnected() const {
  return socket_->IsConnected();
}

void TcpSocketSession::InternalWriteHandler(int32_t error_code) {
  LOG(INFO) << __func__ << " - error_code : " << error_code; 
}


}  // namespace io
}  // namespace nlink

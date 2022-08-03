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
  socket_->Read(&read_buffer_,
    [this](int32_t res) {
      this->InternalReadHandler(read_buffer_, res);
    });
}

// TODO(issuh): should change task
void TcpSocketSession::Write(const base::Buffer& buffer) {
  size_t writed_buffer_size = 0;
  size_t buffer_size = buffer.Size();
  while (writed_buffer_size < buffer_size) {
    size_t send_size =
      kDefaultBufferSize < buffer_size ? kDefaultBufferSize : buffer_size;

    base::Buffer temp(
      buffer.RawData() + writed_buffer_size,
      buffer.RawData() + writed_buffer_size + send_size);

    socket_->Write(&temp,
      [this](int32_t error_code) {
        // this->InternalWriteHandler(error_code);
      });

    writed_buffer_size += send_size;
  }

  // InternalWriteHandler(writed_buffer_size);
}

void TcpSocketSession::Write(
  std::shared_ptr<base::Buffer> buffer) {
  // size_t writed_buffer_size = 0;
  // size_t buffer_size = buffer->Size();
  // while (writed_buffer_size < buffer_size) {
  //   size_t send_size =
  //     kDefaultBufferSize < buffer_size ? kDefaultBufferSize : buffer_size;

  //   base::Buffer temp(
  //     buffer->RawData() + writed_buffer_size,
  //     buffer->RawData() + writed_buffer_size + send_size);

  //   socket_->Write(&temp,
  //     [this](int32_t writed_size) {
  //       this->InternalWriteHandler(writed_size);
  //     });

  //   writed_buffer_size += send_size;
  // }

  // InternalWriteHandler(buffer_size - writed_buffer_size, writed_buffer_size);

  DoWrite(buffer, 0);
}

void TcpSocketSession::DoWrite(
  std::shared_ptr<base::Buffer> buffer,
  int32_t clumulative_trasmission_size) {
  if (clumulative_trasmission_size >= buffer->Size()) {
    return;
  }

  size_t send_size =
    kDefaultBufferSize < buffer->Size() ? kDefaultBufferSize : buffer->Size();

  base::Buffer temp(
    buffer->RawData() + clumulative_trasmission_size,
    buffer->RawData() + clumulative_trasmission_size + send_size);

  socket_->Write(&temp,
    [this, buffer, clumulative_trasmission_size](int32_t writed_size) {
      this->InternalWriteHandler(
        buffer, clumulative_trasmission_size, writed_size);
    });
}

bool TcpSocketSession::IsConnected() const {
  if (nullptr == socket_) {
    return false;
  }
  return socket_->IsConnected();
}

SocketDescriptor TcpSocketSession::SessionId() const {
  return socket_->Descriptor();
}

void TcpSocketSession::InternalWriteHandler(
  std::shared_ptr<base::Buffer> buffer,
  int32_t clumulative_trasmission_size,
  int32_t writed_size) {
  if (0 <= writed_size) {
    clumulative_trasmission_size += writed_size;
    if (clumulative_trasmission_size >= buffer->Size()) {
      if (!write_handler_) {
        return;
      }
      write_handler_(clumulative_trasmission_size);
    } else {
      DoWrite(buffer, clumulative_trasmission_size);
    }
  } else {
    DoWrite(buffer, clumulative_trasmission_size);
  }
}

void TcpSocketSession::InternalReadHandler(
  const base::Buffer& buffer, int32_t size) {
  LOG(INFO) << __func__ << " - res : " << size;
  if (size > 0) {
    if (!read_handler_) {
      return;
    }
    read_handler_(buffer, shared_from_this());
  }

  if (size == 0) {
    Close();
  }
}

}  // namespace io
}  // namespace nlink

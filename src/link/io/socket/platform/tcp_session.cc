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
  base::TaskRunner* task_runner, std::unique_ptr<TcpSocket> socket)
  : task_runner_(task_runner),
    socket_(std::move(socket)),
    is_opend_(false) {
}

TcpSocketSession::~TcpSocketSession() {
  if (nullptr != socket_) {
    socket_->Close();
  }
}

void TcpSocketSession::Open(
  handler::ReadHandler read_handler,
  handler::WriteHandler write_handler,
  handler::CloseHandler close_handler) {
  read_handler_ = read_handler;
  write_handler_ = write_handler;
  close_handler_ = close_handler;

  is_opend_ = true;
}

void TcpSocketSession::Close() {
  close_handler_(shared_from_this());
}

void TcpSocketSession::Read() {
  task_runner_->PostTask(
    [this]() { this->DoRead(); });
}

void TcpSocketSession::Write(std::shared_ptr<base::Buffer> buffer) {
  task_runner_->PostTask(
    [this, buffer]() {
      this->DoWrite(buffer, 0);
    });
}

void TcpSocketSession::DoRead() {
  if (!IsConnected()) {
    return;
  }

  std::shared_ptr<base::Buffer> buffer =
    std::make_shared<base::Buffer>(kDefaultBufferSize);

  socket_->Read(buffer.get(),
    [this, buffer](int32_t res) {
      this->InternalReadHandler(buffer, res);
    });
}

void TcpSocketSession::DoWrite(
  std::shared_ptr<base::Buffer> buffer,
  size_t clumulative_trasmission_size) {
  if (!IsConnected()) {
    return;
  }

  if (clumulative_trasmission_size >= buffer->Size()) {
    return;
  }

  size_t remaind_buffer_size = buffer->Size() - clumulative_trasmission_size;
  size_t will_send_size =
    kDefaultBufferSize < remaind_buffer_size
      ? kDefaultBufferSize : remaind_buffer_size;

  base::Buffer temp(
    buffer->RawData() + clumulative_trasmission_size,
    buffer->RawData() + clumulative_trasmission_size + will_send_size);

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
  return is_opend_ && socket_->IsConnected();
}

SocketDescriptor TcpSocketSession::SessionId() const {
  return socket_->Descriptor();
}

void TcpSocketSession::InternalWriteHandler(
  std::shared_ptr<base::Buffer> buffer,
  size_t clumulative_trasmission_size,
  int32_t writed_size) {
  // LOG(INFO) << __func__ << " - "
  //           << " trasmission_size : " << clumulative_trasmission_size
  //           << ", writed_size : " << writed_size;

  if (0 > writed_size) {
    task_runner_->PostTask(
      [this, buffer, clumulative_trasmission_size]() {
        this->DoWrite(buffer, clumulative_trasmission_size);
      });
    return;
  }

  clumulative_trasmission_size += writed_size;
  if (clumulative_trasmission_size >= buffer->Size()) {
    if (!write_handler_) {
      return;
    }

    task_runner_->PostTask(
      [this, write_handler = write_handler_, clumulative_trasmission_size]() {
        write_handler(clumulative_trasmission_size);
      });
  } else {
    task_runner_->PostTask(
      [this, buffer, clumulative_trasmission_size]() {
        this->DoWrite(buffer, clumulative_trasmission_size);
      });
  }
}

void TcpSocketSession::InternalReadHandler(
  std::shared_ptr<base::Buffer> buffer, int32_t size) {
  LOG(INFO) << __func__ << " - res : " << size;

  std::weak_ptr<TcpSocketSession> session_weak =
    shared_from_this();

  if (0 > size) {
    task_runner_->PostTask(
      [session_weak]() {
        auto session = session_weak.lock();
        if (nullptr != session) {
          session->Close();
        }
      });
  } else if (0 == size) {
    task_runner_->PostTask(
      [session_weak]() {
        auto session = session_weak.lock();
        if (nullptr != session) {
          session->Close();
        }
      });
  } else {
    if (!read_handler_) {
      LOG(WARNING) << __func__ << " - need read handler";
      return;
    }

    buffer->Resize(size);
    task_runner_->PostTask(
      [this, read_handler = read_handler_, buffer]() {
        read_handler(*buffer, shared_from_this());
      });
  }
}

}  // namespace io
}  // namespace nlink

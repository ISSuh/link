/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/socket/platform/tcp_socket_client.h"

#include <utility>

#include "link/io/socket/platform/tcp_connector.h"
#include "link/base/logging.h"
#include "link/base/event/event_util.h"

namespace nlink {
namespace io {

TcpSocketClient::TcpSocketClient(std::weak_ptr<base::TaskRunner> task_runner)
  : task_runner_(task_runner),
    channel_delegate_(nullptr),
    session_(nullptr) {
}

TcpSocketClient::~TcpSocketClient() = default;

void TcpSocketClient::Connect(
  IpEndPoint address,
  handler::ConnectHandler connect_handler,
  handler::CloseHandler close_handler) {
  connect_handler_ = std::move(connect_handler);
  close_handler_ = std::move(close_handler);

  if (nullptr != connector_) {
    connector_->Connect(address,
      [this](std::shared_ptr<Session> session) {
        this->InternalConnectHandler(session);
      });
  }
}

void TcpSocketClient::Disconnect() {
  if (nullptr == session_) {
    return;
  }
  session_->Close();
}

void TcpSocketClient::Write(std::shared_ptr<base::Buffer> buffer) {
  if (nullptr == session_ || buffer->IsEmpty()) {
    LOG(WARNING) << "[TcpSocketClient::Write] cannot write. "
                 << " session is nullptr or buffer size is empty";
    return;
  }

  session_->Write(buffer);
}

bool TcpSocketClient::IsConnected() const {
  if (nullptr == session_) {
    return false;
  }
  return session_->IsConnected();
}

void TcpSocketClient::RegistIOHandler(
  handler::ReadHandler read_handler,
  handler::WriteHandler write_handler) {
  read_handler_ = std::move(read_handler);
  write_handler_ = std::move(write_handler);
}

void TcpSocketClient::OpenChannel(
  base::EventChannel::EventChannelDelegate* delegate) {
  if (nullptr == delegate) {
    return;
  }

  channel_delegate_ = delegate;

  if (nullptr == connector_) {
    connector_.reset(new TcpConnector(
      task_runner_,
      [this](SocketDescriptor descriptor) {
        this->RegistChannel(descriptor);
      }));
  }
}

void TcpSocketClient::CloseChannel() {
  Disconnect();
}

void TcpSocketClient::HandleEvent(const base::Event& event) {
  for (auto type : event.Types()) {
    LOG(INFO) << __func__ << " type : " << EventTypeToString(type);
  }

  for (auto& type : event.Types()) {
    switch (type) {
      case base::Event::Type::READ:
        HandleReadEvent();
        break;
      case base::Event::Type::WRITE:
        HandlerWriteEvent();
        break;
      case base::Event::Type::CLOSE:
      case base::Event::Type::ERROR:
        Disconnect();
        break;
      default:
        LOG(WARNING) << "[TcpSocketClient::ProcessEvent] invalid event type. "
                    << base::EventTypeToString(type);
        break;
    }
  }
}

void TcpSocketClient::HandleReadEvent() {
  if (nullptr == session_) {
    return;
  }
  session_->Read();
}

void TcpSocketClient::HandlerWriteEvent() {
  if (wrtie_task_queue_.empty()) {
    return;
  }

  base::TaskCallback callback = std::move(wrtie_task_queue_.front());
  wrtie_task_queue_.pop();

  std::shared_ptr<base::TaskRunner> task_runner = task_runner_.lock();
  if (task_runner) {
    task_runner->PostTask(std::move(callback));
  }
}

void TcpSocketClient::InternalConnectHandler(std::shared_ptr<Session> session) {
  session_ = session;

  session_->Open(
    [this](const base::Buffer& buffer, std::shared_ptr<Session> session) {
      this->InternalReadHandler(buffer, session);
    },
    [this](size_t length) {
      this->InternalWriteHandler(length);
    },
    [this](std::shared_ptr<Session> session) {
      this->InternalCloseHandler(session);
    });

  if (connect_handler_) {
    connect_handler_(session);
  }
}

void TcpSocketClient::InternalCloseHandler(std::shared_ptr<Session> session) {
  SocketDescriptor descriptor = session->SessionId();
  channel_delegate_->ChannelClosed(descriptor, this);

  if (!close_handler_) {
    return;
  }
  close_handler_(session);
}

void TcpSocketClient::InternalReadHandler(
  const base::Buffer& buffer, std::shared_ptr<io::Session> session) {

  if (!read_handler_) {
    return;
  }
  read_handler_(buffer, session);
}

void TcpSocketClient::InternalWriteHandler(size_t length) {
  if (!write_handler_) {
    return;
  }
  write_handler_(length);
}

void TcpSocketClient::RegistChannel(SocketDescriptor descriptor) {
  channel_delegate_->ChannelOpend(descriptor, this);
}

}  // namespace io
}  // namespace nlink

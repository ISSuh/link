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

TcpSocketClient::TcpSocketClient(base::TaskRunner* task_runner)
  : task_runner_(task_runner),
    dispatcher_context_(nullptr),
    session_(nullptr) {
}

TcpSocketClient::~TcpSocketClient() {
}

void TcpSocketClient::Connect(
  IpEndPoint address,
  handler::ConnectHandler connect_handler,
  handler::CloseHandler close_handler) {
  connect_handler_ = connect_handler;
  close_handler_ = close_handler;

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

void TcpSocketClient::Write(const base::Buffer& buffer) {
  if (nullptr == session_ || buffer.IsEmpty()) {
    return;
  }
  session_->Write(buffer);
}

void TcpSocketClient::Write(
  const base::Buffer& buffer,
  handler::WriteHandler write_handler,
  handler::ReadHandler read_handler) {
  if (nullptr == session_ || buffer.IsEmpty()) {
    return;
  }
  session_->Write(buffer, write_handler, read_handler);
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
  read_handler_ = read_handler;
  write_handler_ = write_handler;
}

void TcpSocketClient::OpenChannel(base::DispatcherConext* context) {
  if (nullptr == context) {
    return;
  }

  dispatcher_context_ = context;

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
  for (auto& type : event.Types()) {
    LOG(INFO) << __func__ << " type : " << EventTypeToString(type);
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

  connect_handler_(session);
}

void TcpSocketClient::InternalCloseHandler(std::shared_ptr<Session> session) {
  close_handler_(session);
}

void TcpSocketClient::InternalReadHandler(
  const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
  read_handler_(buffer, session);
}

void TcpSocketClient::InternalWriteHandler(size_t length) {
  write_handler_(length);
}

void TcpSocketClient::RegistChannel(SocketDescriptor descriptor) {
  if (!dispatcher_context_->Regist(descriptor, this)) {
    LOG(ERROR) << "[TcpSocketClient::RegistChannel] can not regist channel";
  }
}

}  // namespace io
}  // namespace nlink

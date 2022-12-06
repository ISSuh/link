/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/socket/platform/tcp_socket_server.h"

#include <string>
#include <utility>

#include "link/base/logging.h"
#include "link/io/socket/platform/tcp_acceptor.h"
#include "link/base/event/event_util.h"

namespace nlink {
namespace io {

TcpSocketServer::TcpSocketServer(base::TaskRunner* task_runner)
  : task_runner_(task_runner),
    acceptor_(nullptr),
    accept_descriptor_(kInvalidSocket) {
}

TcpSocketServer::~TcpSocketServer() = default;

bool TcpSocketServer::Listen(
  const IpEndPoint& address,
  handler::AcceptHandler accept_handler,
  handler::CloseHandler close_handler) {
  accept_handler_ = std::move(accept_handler);
  close_handler_ = std::move(close_handler);
  return acceptor_->Listen(address);
}

void TcpSocketServer::Accept() {
  acceptor_->Accept(
      [this](std::shared_ptr<Session> session) {
        this->InternalAcceptHandler(session);
    });
}

void TcpSocketServer::Close() {
  CloseAllSessions();
  acceptor_->Close();
}

void TcpSocketServer::RegistIOHandler(
  handler::ReadHandler read_handler,
  handler::WriteHandler write_handler) {
  read_handler_ = std::move(read_handler);
  write_handler_ = std::move(write_handler);
}

void TcpSocketServer::OpenChannel(
  base::EventChannel::EventChannelDelegate* delegate) {
  if (nullptr == delegate) {
    return;
  }

  event_channel_delegate_ = delegate;

  if (nullptr == acceptor_) {
    acceptor_.reset(new TcpAcceptor(task_runner_,
      [this](SocketDescriptor descriptor, bool is_accept_socket_descriptor) {
        this->RegistChannel(descriptor, is_accept_socket_descriptor);
      }));
  }
}

void TcpSocketServer::CloseChannel() {
  CloseAllSessions();
}

void TcpSocketServer::HandleEvent(const base::Event& event) {
  // for (auto type : event.Types()) {
  //   LOG(INFO) << __func__ << " type : " << EventTypeToString(type);
  // }

  if (event.Descriptor() == accept_descriptor_) {
    Accept();
    return;
  }

  SocketDescriptor descriptor = event.Descriptor();
  for (auto& type : event.Types()) {
    switch (type) {
      case base::Event::Type::READ:
        HandleReadEvent(descriptor);
        break;
      case base::Event::Type::WRITE:
        HandlerWriteEvent(descriptor);
        break;
      case base::Event::Type::CLOSE:
      case base::Event::Type::ERROR:
        HandleCloseEvent(descriptor);
        break;
      default:
        LOG(WARNING) << "[TcpSocketServer::ProcessEvent] invalid event type. "
                    << base::EventTypeToString(type);
        break;
    }
  }
}

void TcpSocketServer::HandleReadEvent(SocketDescriptor descriptor) {
  if (sessions_.find(descriptor) == sessions_.end()) {
    LOG(WARNING) << "[TcpSocketServer::HandleReadEvent] invalid descriptor. "
                 << descriptor;
    return;
  }

  std::shared_ptr<Session> session = sessions_.at(descriptor);
  if (session->IsConnected()) {
    session->Read();
  }
}

void TcpSocketServer::HandlerWriteEvent(SocketDescriptor descriptor) {
  if (sessions_.find(descriptor) == sessions_.end()) {
    LOG(WARNING) << "[TcpSocketServer::HandlerWriteEvent] invalid descriptor. "
                 << descriptor;
    return;
  }
}

void TcpSocketServer::HandleCloseEvent(SocketDescriptor descriptor) {
  if (sessions_.find(descriptor) == sessions_.end()) {
    LOG(WARNING) << "[TcpSocketServer::HandleCloseEvent] invalid descriptor. "
                 << descriptor;
    return;
  }

  std::shared_ptr<Session> session = sessions_.at(descriptor);
  task_runner_->PostTask(
    [this, session]() {
      session->Close();
    });
}

void TcpSocketServer::RegistAcceptedClient(std::shared_ptr<Client>) {
}

void TcpSocketServer::InternalAcceptHandler(std::shared_ptr<Session> session) {
  session->Open(
    [this](const base::Buffer& buffer, std::shared_ptr<Session> session) {
      this->InternalReadHandler(buffer, session);
    },
    [this](size_t length) {
      this->InternalWriteHandler(length);
    },
    [this](std::shared_ptr<Session> session) {
      this->InternalCloseHandler(session);
    });

  sessions_.insert({session->SessionId(), session});

  if (accept_handler_) {
    accept_handler_(session);
  }
}

void TcpSocketServer::InternalCloseHandler(std::shared_ptr<Session> session) {
  CloseSession(session->SessionId());

  if (close_handler_) {
    close_handler_(session);
  }
}

void TcpSocketServer::InternalReadHandler(
  const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
  if (read_handler_) {
    read_handler_(buffer, session);
  }

  SocketDescriptor descriptor = session->SessionId();
  event_channel_delegate_->UpdateChannel(descriptor, this);
}

void TcpSocketServer::InternalWriteHandler(size_t length) {
  if (write_handler_) {
    write_handler_(length);
  }
}

void TcpSocketServer::RegistChannel(
  SocketDescriptor descriptor, bool is_accept_socket_descriptor) {
  if (is_accept_socket_descriptor) {
    accept_descriptor_ = descriptor;
  }

  event_channel_delegate_->ChannelOpend(descriptor, this);
}

void TcpSocketServer::CloseSession(SocketDescriptor descriptor) {
  if (sessions_.find(descriptor) == sessions_.end()) {
    LOG(WARNING) << "[TcpSocketServer::CloseSession] invalid descriptor. "
                 << descriptor;
    return;
  }

  event_channel_delegate_->ChannelClosed(descriptor, this);

  std::shared_ptr<Session> session = sessions_.at(descriptor);
  sessions_.erase(descriptor);
}

void TcpSocketServer::CloseAllSessions() {
  for (const auto& item : sessions_) {
    SocketDescriptor descriptor = item.first;
    std::shared_ptr<Session> session = item.second;

    session->Close();
    event_channel_delegate_->ChannelClosed(descriptor, this);
  }
  sessions_.clear();
}

}  // namespace io
}  // namespace nlink

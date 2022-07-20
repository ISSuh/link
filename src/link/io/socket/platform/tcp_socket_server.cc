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
  : task_runner_(task_runner), acceptor_(nullptr) {
}

TcpSocketServer::~TcpSocketServer() {
}

bool TcpSocketServer::Listen(
  const IpEndPoint& address,
  handler::AcceptHandler accept_handler,
  handler::CloseHandler close_handler) {
  accept_handler_ = accept_handler;
  close_handler_ = close_handler;
  return acceptor_->Listen(address);
}

void TcpSocketServer::Accept(
  handler::AcceptHandler accept_handler,
  handler::CloseHandler close_handler) {
}

void TcpSocketServer::Close() {
  CloseAllSessions();
  acceptor_->Close();
}

void TcpSocketServer::RegistIOHandler(
  handler::ReadHandler read_handler,
  handler::WriteHandler write_handler) {
  read_handler_ = read_handler;
  write_handler_ = write_handler;
}

void TcpSocketServer::OpenChannel(base::DispatcherConext* context) {
  if (!context) {
    return;
  }

  dispatcher_context_ = context;

  if (nullptr == acceptor_) {
    acceptor_.reset(new TcpAcceptor(task_runner_,
      [this](SocketDescriptor descriptor) {
        this->RegistChannel(descriptor);
      },
      [](std::shared_ptr<Client>) {}));
  }
}

void TcpSocketServer::CloseChannel() {
  Close();
}

void TcpSocketServer::HandleEvent(const base::Event& event) {
  for (auto type : event.Types()) {
    LOG(INFO) << __func__ << " type : " << EventTypeToString(type);
  }

  for (auto& type : event.Types()) {
    base::TaskCallback callback = {};
    switch (type) {
      case base::Event::Type::ACCEPT:

      case base::Event::Type::READ:
        acceptor_->Accept(
          [this](std::shared_ptr<Session> session) {
            this->InternalAcceptHandler(session);
          });
        break;
        break;
      case base::Event::Type::WRITE:
        break;
      case base::Event::Type::CLOSE:
        CloseAllSessions();
        break;
      case base::Event::Type::ERROR:
        CloseAllSessions();
        break;
      default:
        LOG(WARNING) << "[TcpSocketServer::ProcessEvent] invalid event type. "
                    << base::EventTypeToString(type);
        break;
    }
  }
}

void TcpSocketServer::RegistAcceptedClient(std::shared_ptr<Client> client) {
  clients_.insert(client);
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

  if (!accept_handler_) {
    return;
  }
  accept_handler_(session);
}

void TcpSocketServer::InternalCloseHandler(std::shared_ptr<Session> session) {
  sessions_.erase(session);

  if (!close_handler_) {
    return;
  }
  close_handler_(session);
}

void TcpSocketServer::InternalReadHandler(
  const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
  if (!read_handler_) {
    return;
  }
  read_handler_(buffer, session);
}

void TcpSocketServer::InternalWriteHandler(size_t length) {
  if (!write_handler_) {
    return;
  }
  write_handler_(length);
}

void TcpSocketServer::RegistChannel(SocketDescriptor descriptor) {
  LOG(INFO) << __func__ << " - descriptor : " << descriptor;
  if (!dispatcher_context_->Regist(descriptor, this)) {
    LOG(ERROR) << "[TcpSocketClient::RegistChannel] can not regist channel";
  }
}

void TcpSocketServer::CloseAllSessions() {
  for (auto session : sessions_) {
    session->Close();
  }
  sessions_.clear();
}

}  // namespace io
}  // namespace nlink

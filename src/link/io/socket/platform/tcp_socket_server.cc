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
      [this](SocketDescriptor descriptor, bool is_accept_socket_descriptor) {
        this->RegistChannel(descriptor, is_accept_socket_descriptor);
      }));
  }
}

void TcpSocketServer::CloseChannel() {
  Close();
}

void TcpSocketServer::HandleEvent(const base::Event& event) {
  for (auto type : event.Types()) {
    LOG(INFO) << __func__ << " type : " << EventTypeToString(type);
  }

  if (event.Discriptor() == accept_descriptor_) {
    acceptor_->Accept(
      [this](std::shared_ptr<Session> session) {
        this->InternalAcceptHandler(session);
      });

    return;
  }

  SocketDescriptor descriptor = event.Discriptor();
  for (auto& type : event.Types()) {
    base::TaskCallback callback = {};
    switch (type) {
      case base::Event::Type::READ:
        HandleReadEvent(descriptor);
        break;
      case base::Event::Type::WRITE:
        HandlerWriteEvent(descriptor);
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

void TcpSocketServer::HandleReadEvent(SocketDescriptor descriptor) {
  if (sessions_.find(descriptor) == sessions_.end()) {
    LOG(WARNING) << "[TcpSocketServer::HandleReadEvent] invalid descriptor. "
                 << descriptor;
    return;
  }

  std::shared_ptr<Session> session = sessions_.at(descriptor);
  session->Read(nullptr);
  // task_runner_->PostTask(
  //   [this, session = session_]() {
  //     session->Read(nullptr);
  //   });
}

void TcpSocketServer::HandlerWriteEvent(SocketDescriptor descriptor) {
  if (sessions_.find(descriptor) == sessions_.end()) {
    LOG(WARNING) << "[TcpSocketServer::HandlerWriteEvent] invalid descriptor. "
                 << descriptor;
    return;
  }

  if (wrtie_task_queue_.empty()) {
    return;
  }

  auto callback = wrtie_task_queue_.front();
  wrtie_task_queue_.pop();

  task_runner_->PostTask(callback);
}

void TcpSocketServer::RegistAcceptedClient(std::shared_ptr<Client> client) {
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
  sessions_.erase(session->SessionId());

  if (close_handler_) {
    close_handler_(session);
  }
}

void TcpSocketServer::InternalReadHandler(
  const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
  if (read_handler_) {
    read_handler_(buffer, session);
  }
}

void TcpSocketServer::InternalWriteHandler(size_t length) {
  if (write_handler_) {
    write_handler_(length);
  }
}

void TcpSocketServer::RegistChannel(
  SocketDescriptor descriptor, bool is_accept_socket_descriptor) {
  LOG(INFO) << __func__ << " - descriptor : " << descriptor;

  if (is_accept_socket_descriptor) {
    accept_descriptor_ = descriptor;
  }

  if (!dispatcher_context_->Regist(descriptor, this)) {
    LOG(ERROR) << "[TcpSocketClient::RegistChannel] can not regist channel";
  }
}

void TcpSocketServer::CloseSession(SocketDescriptor descriptor) {
  if (sessions_.find(descriptor) == sessions_.end()) {
    LOG(WARNING) << "[TcpSocketServer::CloseSession] invalid descriptor. "
                 << descriptor;
    return;
  }

  sessions_.erase(descriptor);
}

void TcpSocketServer::CloseAllSessions() {
  sessions_.clear();
}

}  // namespace io
}  // namespace nlink

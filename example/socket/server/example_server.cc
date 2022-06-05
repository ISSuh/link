/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "example_server.h"

#include <vector>

#include "link/base/logging.h"

using namespace nlink;

ExampleServer::ExampleServer()
  : handlers_({
      base::Bind(&ExampleServer::OnAccept, this),
      component::SocketComponent::Handler::ConnectHandler(),
      base::Bind(&ExampleServer::OnClose, this),
      base::Bind(&ExampleServer::OnRead, this),
      base::Bind(&ExampleServer::OnWrite, this),
    }),
    server_component_(nullptr) {}

ExampleServer::~ExampleServer() = default;

void ExampleServer::CreateAndRegistComponent(
  nlink::base::TaskRunner* task_runner,
  nlink::handle::LinkHandle* handle) {
  server_component_ =
      component::TcpServerComponent::CreateComponent(task_runner, handlers_);

  handle->RegistComponent(server_component_);
}

void ExampleServer::ServerOpen(const std::string& address, int32_t port) {
  server_component_->Open(address, port);
}

void ExampleServer::OnAccept(std::shared_ptr<nlink::io::Session> session) {
  LOG(INFO) << "[ExampleServer::OnAccept]"
            << " session : " << session.get();
}

void ExampleServer::OnClose(std::shared_ptr<nlink::io::Session> session) {
  LOG(INFO) << "[ExampleServer::OnClose]"
            << " session : " << session.get();
}

void ExampleServer::OnRead(
  const nlink::base::Buffer& buffer,
  std::shared_ptr<nlink::io::Session> session) {
  const std::vector<uint8_t>& received_data = buffer.Data();
  std::string received(received_data.begin(), received_data.end());
  LOG(INFO) << "[ExampleServer::OnRead]"
            << " received : " << buffer.Size();

  // session->Write(buffer);
}

void ExampleServer::OnWrite(size_t lengeh) {
  // LOG(INFO) << "[ExampleServer::OnWrite]"
  //           << " lengeh : " << lengeh;
}

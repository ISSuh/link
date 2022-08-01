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
      [this](std::shared_ptr<nlink::io::Session> session) {
        this->OnAccept(session);
      },
      component::SocketComponent::Handler::ConnectHandler(),
      [this](std::shared_ptr<nlink::io::Session> session) {
        this->OnClose(session);
      },
      [this](
        const nlink::base::Buffer& buffer,
        std::shared_ptr<nlink::io::Session> session) {
        this->OnRead(buffer, session);
      },
      [this](size_t length) {
        this->OnWrite(length);
      }}),
    server_component_(nullptr) {}

ExampleServer::~ExampleServer() = default;

void ExampleServer::CreateAndRegistComponent(
  nlink::base::TaskRunner* task_runner,
  nlink::handle::LinkHandle* handle) {
  auto component_factory_weak = handle->ComponentFactory();
  auto component_factory = component_factory_weak.lock();

  server_component_ =
    component_factory->CreateTcpServerComponent(task_runner, handlers_);
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
  static int32_t receiver_size = 0;

  receiver_size += buffer.Size();

  const std::vector<uint8_t>& received_data = buffer.Data();
  std::string received(received_data.begin(), received_data.end());
  LOG(INFO) << "[ExampleServer::OnRead]"
            << " received : " << buffer.Size()
            << " total : " << receiver_size;

  // session->Write(buffer);
}

void ExampleServer::OnWrite(size_t lengeh) {
  // LOG(INFO) << "[ExampleServer::OnWrite]"
  //           << " lengeh : " << lengeh;
}

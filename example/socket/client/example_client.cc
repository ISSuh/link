/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "example_client.h"

#include "link/base/logging.h"

using namespace nlink;

ExampleClient::ExampleClient()
  : handlers_({
      component::SocketComponent::Handler::AcceptHandler(),
      base::Bind(&ExampleClient::OnConnect, this),
      base::Bind(&ExampleClient::OnClose, this),
      base::Bind(&ExampleClient::OnRead, this),
      base::Bind(&ExampleClient::OnWrite, this),
    }),
    client_component_(
      component::TcpClientComponent::CreateComponent(handlers_)) {}

ExampleClient::~ExampleClient() = default;

void ExampleClient::RegistComponent(nlink::handle::LinkHandle* handle) {
  handle->RegistComponent(client_component_);
}

void ExampleClient::Connect(const std::string& address, int32_t port) {
  client_component_->Connect(address, port);
}

void ExampleClient::Write(const std::string& message) {
  base::Buffer buffer(message);
  client_component_->Write(buffer);
}

void ExampleClient::OnConnect(std::shared_ptr<nlink::net::Session> session) {
  LOG(INFO) << "[ExampleClient::OnConnect]"
            << " session : " << session.get();
}

void ExampleClient::OnClose(std::shared_ptr<nlink::net::Session> session) {
  LOG(INFO) << "[ExampleClient::OnClose]"
            << " session : " << session.get();
}

void ExampleClient::OnRead(const nlink::base::Buffer& buffer) {
  LOG(INFO) << "[ExampleClient::OnRead]"
            << " size : " << buffer.Size();
}

void ExampleClient::OnWrite(size_t lengeh) {
  LOG(INFO) << "[ExampleClient::OnWrite]"
            << " lengeh : " << lengeh;
}

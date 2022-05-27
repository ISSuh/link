/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "example_server.h"

#include "link/base/logging.h"

ExampleServer::ExampleServer()
  : server_component_(component::TcpServerComponent::CreateComponent()) {}

ExampleServer::~ExampleServer() = default;

void ExampleServer::RegistComponent(handle::LinkHandle* handle) {
  handle->RegistComponent(server_component_);
}

void ExampleServer::ServerOpen(const std::string& address, int32_t port) {
  server_component_->Open(address, port);
}

void ExampleServer::OnAccept() {

}

void ExampleServer::OnClose() {

}

void ExampleServer::OnWrite() {

}

void ExampleServer::OnRead(const base::Buffer& buffer) {
  std::string temp(buffer.RawData(), buffer.RawData() + buffer.Size());
  LOG(INFO) << __func__ << " - temp : " << temp << " / size : " << temp.size();
}

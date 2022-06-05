/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "example_client.h"

#include <vector>

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
    client_component_(nullptr),
    is_connected(false) {}

ExampleClient::~ExampleClient() = default;

void ExampleClient::CreateAndRegistComponent(
  nlink::base::TaskRunner* task_runner,
  nlink::handle::LinkHandle* handle) {
  client_component_ =
    component::TcpClientComponent::CreateComponent(task_runner, handlers_);

  handle->RegistComponent(client_component_);
}

void ExampleClient::Connect(const std::string& address, int32_t port) {
  client_component_->Connect(address, port);
}

void ExampleClient::Disconnect() {
  client_component_->DisConnect();
}

void ExampleClient::Write(const std::string& message) {
  base::Buffer buffer(message);
  client_component_->Write(buffer);
}

bool ExampleClient::IsConnected() {
  return is_connected;
}

void ExampleClient::OnConnect(std::shared_ptr<nlink::io::Session> session) {
  LOG(INFO) << "[ExampleClient::OnConnect]"
            << " session : " << session.get();

  is_connected = true;
}

void ExampleClient::OnClose(std::shared_ptr<nlink::io::Session> session) {
  LOG(INFO) << "[ExampleClient::OnClose]"
            << " session : " << session.get();
}

void ExampleClient::OnRead(
  const nlink::base::Buffer& buffer,
  std::shared_ptr<nlink::io::Session> session) {
  const std::vector<uint8_t>& received_data = buffer.Data();
  std::string received(received_data.begin(), received_data.end());
  LOG(INFO) << "[ExampleClient::OnRead]"
            << " received : " << buffer.Size();

}

void ExampleClient::OnWrite(size_t lengeh) {
  // LOG(INFO) << "[ExampleClient::OnWrite]"
  //           << " lengeh : " << lengeh;
}

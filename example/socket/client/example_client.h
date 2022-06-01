/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef EXAMPLE_SOCKET_CLIENT_EXAMPLE_CLIENT_H_
#define EXAMPLE_SOCKET_CLIENT_EXAMPLE_CLIENT_H_

#include <string>
#include <memory>

#include <link/base/buffer.h>
#include <link/handle/link_handle.h>
#include <link/component/ipc/socket/tcp_client_component.h>

class ExampleClient {
 public:
  ExampleClient();
  ~ExampleClient();

  void RegistComponent(nlink::handle::LinkHandle* handle);
  void Connect(const std::string& address, int32_t port);
  void Disconnect();
  void Write(const std::string& message);

  bool IsConnected();

 private:
  void OnConnect(std::shared_ptr<nlink::net::Session> session);
  void OnClose(std::shared_ptr<nlink::net::Session> session);
  void OnWrite(size_t lengeh);
  void OnRead(
    const nlink::base::Buffer& buffer,
    std::shared_ptr<nlink::net::Session> session);

  nlink::component::SocketComponent::Handler handlers_;
  nlink::component::TcpClientComponent* client_component_;
  bool is_connected;
};

#endif  // EXAMPLE_SOCKET_CLIENT_EXAMPLE_CLIENT_H_

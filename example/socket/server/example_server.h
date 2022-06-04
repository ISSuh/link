/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef EXAMPLE_SOCKET_SERVER_EXAMPLE_SERVER_H_
#define EXAMPLE_SOCKET_SERVER_EXAMPLE_SERVER_H_

#include <string>
#include <memory>

#include <link/base/buffer.h>
#include <link/base/task/task_runner.h>
#include <link/handle/link_handle.h>
#include <link/component/ipc/socket/tcp_server_component.h>

class ExampleServer {
 public:
  explicit ExampleServer();
  ~ExampleServer();

  void CreateAndRegistComponent(
    nlink::base::TaskRunner* task_runner,
    nlink::handle::LinkHandle* handle);
  void ServerOpen(const std::string& address, int32_t port);

 private:
  void OnAccept(std::shared_ptr<nlink::net::Session> session);
  void OnClose(std::shared_ptr<nlink::net::Session> session);
  void OnWrite(size_t lengeh);
  void OnRead(
    const nlink::base::Buffer& buffer,
    std::shared_ptr<nlink::net::Session> session);

  nlink::component::SocketComponent::Handler handlers_;
  nlink::component::TcpServerComponent* server_component_;
};

#endif  // EXAMPLE_SOCKET_SERVER_EXAMPLE_SERVER_H_

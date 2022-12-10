/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_IPC_SOCKET_TCP_CLIENT_COMPONENT_H_
#define LINK_COMPONENT_IPC_SOCKET_TCP_CLIENT_COMPONENT_H_

#include <string>
#include <memory>
#include <vector>

#include "link/base/macro.h"
#include "link/base/buffer.h"
#include "link/base/task/task_runner.h"
#include "link/component/ipc/socket/socket_component.h"
#include "link/io/socket/client.h"

namespace nlink {
namespace component {

class RpcComponent;

class TcpClientComponent : public SocketComponent {
 public:
  static TcpClientComponent* CreateComponent(
    base::ComponentChannelController* channel_controller,
    std::weak_ptr<base::TaskRunner> task_runner,
    SocketComponent::Handler handlers);

  void Connect(const std::string& address, int32_t port);
  void Disconnect();

  void Write(std::shared_ptr<base::Buffer> buffer);

  bool IsConnected() const;

 private:
  TcpClientComponent(
    base::ComponentChannelController* channel_controller,
    std::weak_ptr<base::TaskRunner> task_runner,
    SocketComponent::Handler handlers);
  virtual ~TcpClientComponent();

  void InternalConnectHandler(std::shared_ptr<io::Session> session);
  void InternalCloseHandler(std::shared_ptr<io::Session> session);
  void InternalReadHandler(
    const base::Buffer& buffer, std::shared_ptr<io::Session> session);
  void InternalWriteHandler(size_t length);

  std::unique_ptr<io::Client> client_;
  SocketComponent::Handler::ConnectHandler connect_handler_;
  SocketComponent::Handler::CloseHandler close_handler_;
  SocketComponent::Handler::ReadHandler read_handler_;
  SocketComponent::Handler::WriteHandler write_handler_;

  DISAALOW_COPY_AND_ASSIGN(TcpClientComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_IPC_SOCKET_TCP_CLIENT_COMPONENT_H_

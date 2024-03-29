/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_IPC_SOCKET_TCP_SERVER_COMPONENT_H_
#define LINK_COMPONENT_IPC_SOCKET_TCP_SERVER_COMPONENT_H_

#include <string>
#include <memory>

#include "link/base/macro.h"
#include "link/base/buffer.h"
#include "link/base/task/task_runner.h"
#include "link/component/ipc/socket/socket_component.h"
#include "link/io/socket/server.h"

namespace nlink {
namespace component {

class SocketComponent;

class TcpServerComponent : public SocketComponent {
 public:
  static TcpServerComponent* CreateComponent(
    base::ComponentChannelController* channel_controller,
    std::weak_ptr<base::TaskRunner> task_runner,
    SocketComponent::Handler handlers);

  void Open(const std::string& address, int32_t port);
  void Close();

 private:
  explicit TcpServerComponent(
    base::ComponentChannelController* channel_controller,
    std::weak_ptr<base::TaskRunner> task_runner,
    SocketComponent::Handler handlers);
  virtual ~TcpServerComponent();

  void InternalAcceptHandler(std::shared_ptr<io::Session> session);
  void InternalCloseHandler(std::shared_ptr<io::Session> session);
  void InternalReadHandler(
    const base::Buffer& buffer, std::shared_ptr<io::Session> session);
  void InternalWriteHandler(size_t length);

  std::unique_ptr<io::Server> server_;
  SocketComponent::Handler::AcceptHandler accept_handler_;
  SocketComponent::Handler::CloseHandler close_handler_;
  SocketComponent::Handler::ReadHandler read_handler_;
  SocketComponent::Handler::WriteHandler write_handler_;

  DISAALOW_COPY_AND_ASSIGN(TcpServerComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_IPC_SOCKET_TCP_SERVER_COMPONENT_H_

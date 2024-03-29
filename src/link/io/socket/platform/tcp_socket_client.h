/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_IO_SOCKET_PLATFORM_TCP_SOCKET_CLIENT_H_
#define LINK_IO_SOCKET_PLATFORM_TCP_SOCKET_CLIENT_H_

#include <memory>
#include <vector>
#include <queue>

#include "link/base/macro.h"
#include "link/base/buffer.h"
#include "link/base/callback.h"
#include "link/base/task/task_runner.h"
#include "link/base/event/event_dispatcher.h"
#include "link/io/base/ip_endpoint.h"
#include "link/io/socket/client.h"
#include "link//io/socket/connector.h"
#include "link/io/socket/session.h"
#include "link/io/socket/platform/socket_descripor.h"

namespace nlink {
namespace io {

class TcpSocketClient : public Client {
 public:
  using EventTaskQueue = std::queue<base::TaskCallback>;

  explicit TcpSocketClient(std::weak_ptr<base::TaskRunner> task_runner);
  virtual ~TcpSocketClient();

  // Client
  void Connect(
    IpEndPoint endpoint,
    handler::ConnectHandler connect_handler,
    handler::CloseHandler close_handler) override;
  void Disconnect() override;
  void RegistIOHandler(
    handler::ReadHandler read_handler,
    handler::WriteHandler write_handler) override;
  void Write(std::shared_ptr<base::Buffer> buffer) override;

  bool IsConnected() const override;

  // EventChannel
  void OpenChannel(base::EventChannel::EventChannelDelegate* delegate) override;
  void CloseChannel() override;
  void HandleEvent(const base::Event& event) override;

 private:
  void HandleReadEvent();
  void HandlerWriteEvent();

  void InternalConnectHandler(std::shared_ptr<Session> session);
  void InternalCloseHandler(std::shared_ptr<Session> session);
  void InternalReadHandler(
    const base::Buffer& buffer, std::shared_ptr<io::Session> session);
  void InternalWriteHandler(size_t length);

  void RegistChannel(SocketDescriptor descriptor);

  std::weak_ptr<base::TaskRunner> task_runner_;
  base::EventChannel::EventChannelDelegate* channel_delegate_;

  std::unique_ptr<Connector> connector_;
  std::shared_ptr<Session> session_;

  EventTaskQueue wrtie_task_queue_;

  handler::ConnectHandler connect_handler_;
  handler::CloseHandler close_handler_;
  handler::ReadHandler read_handler_;
  handler::WriteHandler write_handler_;

  bool try_connecting_;

  DISAALOW_COPY_AND_ASSIGN(TcpSocketClient)
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_PLATFORM_TCP_SOCKET_CLIENT_H_

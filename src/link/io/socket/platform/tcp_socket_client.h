/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_IO_SOCKET_PLATFORM_TCP_SOCKET_CLIENT_H_
#define LINK_IO_SOCKET_PLATFORM_TCP_SOCKET_CLIENT_H_

#include <memory>
#include <vector>

#include "link/base/macro.h"
#include "link/base/buffer.h"
#include "link/base/task/task_runner.h"
#include "link/io/base/ip_endpoint.h"
#include "link/io/socket/client.h"
#include "link/io/socket/session.h"
#include "link/io/socket/platform/tcp_socket.h"

namespace nlink {
namespace io {

class TcpSocketClient : public Client {
 public:
  explicit TcpSocketClient(base::TaskRunner* task_runner);
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
  void Write(const base::Buffer& buffer) override;
  void Write(
    const base::Buffer& buffer,
    handler::WriteHandler write_handler,
    handler::ReadHandler read_handler) override;

  // EventChannel
  void OpenChannel(base::DispatcherConext* context) override;
  void CloseChannel() override;
  void HandleEvent(const base::Event& event) override;

 private:
  // void InternalConnectHandler(std::shared_ptr<Session> session);
  void InternalConnectHandler(int32_t error_code);
  void InternalCloseHandler(std::shared_ptr<Session> session);
  void InternalReadHandler(
    const base::Buffer& buffer, std::shared_ptr<io::Session> session);
  void InternalWriteHandler(size_t length);

  base::TaskRunner* task_runner_;

  std::shared_ptr<Session> session_;

  handler::ConnectHandler connect_handler_;
  handler::CloseHandler close_handler_;
  handler::ReadHandler read_handler_;
  handler::WriteHandler write_handler_;

  DISAALOW_COPY_AND_ASSIGN(TcpSocketClient)
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_PLATFORM_TCP_SOCKET_CLIENT_H_
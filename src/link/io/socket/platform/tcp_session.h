/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_IO_SOCKET_PLATFORM_TCP_SESSION_H_
#define LINK_IO_SOCKET_PLATFORM_TCP_SESSION_H_

#include <memory>
#include <string>
#include <vector>

#include "link/base/callback/callback.h"
#include "link/base/task/task_runner.h"
#include "link/io/socket/session.h"
#include "link/io/socket/platform/tcp_socket.h"

namespace nlink {
namespace io {

class TcpSocketSession
  : public Session,
    public std::enable_shared_from_this<TcpSocketSession> {
 public:
  TcpSocketSession(
    base::TaskRunner* task_runner,
    std::shared_ptr<TcpSocket> socket);
  ~TcpSocketSession();

  // Session
  void Open(
    handler::ReadHandler read_handler,
    handler::WriteHandler write_handler,
    handler::CloseHandler close_handler) override;
  void Close() override;
  void Write(const base::Buffer& buffer) override;
  void Write(
    const base::Buffer& buffer,
    handler::WriteHandler write_handler,
    handler::ReadHandler read_handler) override;

  bool IsConnected() const override;

 private:
  void InternalWriteHandler(int32_t error_code);

  base::TaskRunner* task_runner_;

  std::shared_ptr<TcpSocket> socket_;

  handler::ReadHandler read_handler_;
  handler::WriteHandler write_handler_;
  handler::CloseHandler close_handler_;
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_PLATFORM_TCP_SESSION_H_

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
#include <queue>

#include "link/base/callback.h"
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
    base::TaskRunner* task_runner, std::unique_ptr<TcpSocket> socket);
  ~TcpSocketSession();

  // Session
  void Open(
    handler::ReadHandler read_handler,
    handler::WriteHandler write_handler,
    handler::CloseHandler close_handler) override;
  void Close() override;

  void Read() override;
  void Write(std::shared_ptr<base::Buffer> buffer) override;

  bool IsConnected() const override;
  int32_t SessionId() const override;

 private:
  void DoNextProcess();

  void DoClose();
  void DoRead(std::shared_ptr<base::Buffer> buffer);
  void DoWrite(
    std::shared_ptr<base::Buffer> buffer,
    size_t clumulative_trasmission_size);

  void InternalWriteHandler(
    std::shared_ptr<base::Buffer> buffer,
    size_t clumulative_trasmission_size,
    int32_t writed_size);
  void InternalReadHandler(
    std::shared_ptr<base::Buffer> buffer,
    std::shared_ptr<base::Buffer> received_buffer,
    int32_t size);

  base::TaskRunner* task_runner_;
  std::unique_ptr<TcpSocket> socket_;

  handler::ReadHandler read_handler_;
  handler::WriteHandler write_handler_;
  handler::CloseHandler close_handler_;

  std::queue<base::TaskCallback> task_queue_;

  bool is_opend_;
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_PLATFORM_TCP_SESSION_H_

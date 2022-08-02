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
#include "link/io/socket/session.h"
#include "link/io/socket/platform/tcp_socket.h"

namespace nlink {
namespace io {

class TcpSocketSession
  : public Session,
    public std::enable_shared_from_this<TcpSocketSession> {
 public:
  explicit TcpSocketSession(std::unique_ptr<TcpSocket> socket);
  ~TcpSocketSession();

  // Session
  void Open(
    handler::ReadHandler read_handler,
    handler::WriteHandler write_handler,
    handler::CloseHandler close_handler) override;
  void Close() override;

  void Read(base::Buffer* buffer) override;

  void Write(const base::Buffer& buffer) override;
  void Write(
    const base::Buffer& buffer,
    handler::WriteHandler write_handler,
    handler::ReadHandler read_handler) override;
  void Write(std::shared_ptr<base::Buffer> buffer) override;

  bool IsConnected() const override;
  int32_t SessionId() const override;

 private:
  void InternalWriteHandler(int32_t res);
  void InternalReadHandler(const base::Buffer& buffer, int32_t res);

  std::unique_ptr<TcpSocket> socket_;

  base::Buffer read_buffer_;

  handler::ReadHandler read_handler_;
  handler::WriteHandler write_handler_;
  handler::CloseHandler close_handler_;
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_PLATFORM_TCP_SESSION_H_

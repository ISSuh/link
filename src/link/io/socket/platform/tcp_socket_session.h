/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_IO_SOCKET_PLATFORM_TCP_SOCKET_SESSION_H_
#define LINK_IO_SOCKET_PLATFORM_TCP_SOCKET_SESSION_H_

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
  explicit TcpSocketSession();
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
  // void InternalWriteHandler(
  //   handler::WriteHandler write_handler,
  //   std::error_code ec,
  //   std::size_t length);

  void DoRead();
  // void InternalReadHandler(const std::error_code& ec, std::size_t length);

  // asio::ip::tcp::socket socket_;
  std::unique_ptr<TcpSocket> socket_;

  int32_t max_packet_size_;
  std::vector<uint8_t> raw_read_buffer;

  handler::ReadHandler read_handler_;
  handler::WriteHandler write_handler_;
  handler::CloseHandler close_handler_;

};

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_PLATFORM_TCP_SOCKET_SESSION_H_

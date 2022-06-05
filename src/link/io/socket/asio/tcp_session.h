/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_ASIO_TCP_SESSION_H_
#define LINK_NET_SOCKET_ASIO_TCP_SESSION_H_

#include <memory>
#include <string>
#include <vector>

#include "link/base/callback/callback.h"
#include "link/io/socket/session.h"
#include "link/third_party/asio/asio/ip/tcp.hpp"

namespace nlink {
namespace io {

class TcpSession
  : public Session,
    public std::enable_shared_from_this<TcpSession> {
 public:
  explicit TcpSession(asio::ip::tcp::socket socket);
  ~TcpSession();

  // Session
  void Open(
    handler::ReadHandler read_handler,
    handler::WriteHandler write_handler,
    handler::CloseHandler close_handler) override;
  void Close() override;
  void Write(const base::Buffer& buffer) override;

  bool IsConnected() const override;

 private:
  void InternalWriteHandler(std::error_code ec, std::size_t length);

  void DoRead();
  void InternalReadHandler(const std::error_code& ec, std::size_t length);

  asio::ip::tcp::socket socket_;

  int32_t max_packet_size_;
  std::vector<uint8_t> raw_read_buffer;

  handler::ReadHandler read_handler_;
  handler::WriteHandler write_handler_;
  handler::CloseHandler close_handler_;

};

}  // namespace io
}  // namespace nlink

#endif  // LINK_NET_SOCKET_ASIO_TCP_SESSION_H_

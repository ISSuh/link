/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_ASIO_CHANNEL_H_
#define LINK_NET_SOCKET_ASIO_CHANNEL_H_

#include <memory>
#include <string>
#include <vector>

#include "link/base/buffer.h"
#include "link/base/callback/callback.h"
#include "link/net/socket/session.h"
#include "link/net/socket/handler.h"
#include "link/third_party/asio/asio/ip/tcp.hpp"

namespace nlink {
namespace net {

class ClientSideSession
  : public Session,
    public std::enable_shared_from_this<ClientSideSession> {
 public:
  explicit ClientSideSession(asio::ip::tcp::socket socket);
  ~ClientSideSession();

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
  void InternalReadHandler(
    const std::vector<uint8_t>& buffer,
    std::error_code ec, std::size_t length);

  asio::ip::tcp::socket socket_;

  base::Buffer read_buffer_;
  handler::ReadHandler read_handler_;
  handler::WriteHandler write_handler_;
  handler::CloseHandler close_handler_;
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_SOCKET_ASIO_CHANNEL_H_

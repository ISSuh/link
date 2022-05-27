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
#include "link/third_party/asio/asio/ip/tcp.hpp"

namespace nlink {
namespace net {

class ClientSideSession
  : public Session,
    public std::enable_shared_from_this<ClientSideSession> {
 public:
  class Delegate {
    virtual void OnSessionClose() = 0;
  };

  explicit ClientSideSession(asio::ip::tcp::socket socket);
  ~ClientSideSession();

  // Session
  void Open() override;
  void Close() override;
  void Write(const base::Buffer& buffer);

 private:
  void ConnectHandler(const asio::error_code& error);

  // void DoWrite(
    // const std::vector<uint8_t>& buffer, WriteCallback write_callback);
  // void WriteHandler(
  //   WriteCallback write_callback, std::error_code ec, std::size_t length);

  // void DoRead();
  // void ReadHandler(std::error_code ec, std::size_t length);

  asio::ip::tcp::socket socket_;
  Delegate* delegate_;

  base::Buffer read_buffer_;
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_SOCKET_ASIO_CHANNEL_H_

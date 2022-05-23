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

#include "link/base/callback/callback.h"
#include "link/net/socket/server.h"
#include "link/third_party/asio/asio/ip/tcp.hpp"

namespace nlink {
namespace net {

class Session : public std::enable_shared_from_this<Session> {
 public:
  class Delegate {
    virtual void OnSessionClose() = 0;
  };

  using ConnectCallback = base::Callback<void(bool)>;
  using ReadCallback = base::Callback<void(bool)>;
  using WriteCallback = base::Callback<void(std::size_t length)>;

  Session(asio::ip::tcp::socket socket);
  ~Session();

  // server
  void Open(Server::ReadHandler read_handler);
  void Close();

  // client
  void Connect(const std::string& address, int32_t port);
  void Disconnect();
  void Write(const std::vector<uint8_t>& buffer, WriteCallback write_callback);

 private:
  void ConnectHandler(const asio::error_code& error);

  void DoWrite(
    const std::vector<uint8_t>& buffer, WriteCallback write_callback);
  void WriteHandler(
    WriteCallback write_callback, std::error_code ec, std::size_t length);

  void DoRead();
  void ReadHandler(std::error_code ec, std::size_t length);

  ConnectCallback connect_callback_;
  Server::ReadHandler read_callback_;
  WriteCallback write_callback_;

  asio::ip::tcp::socket socket_;
  // Delegate* delegate_;

  std::vector<uint8_t> read_buffer_;
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_SOCKET_ASIO_CHANNEL_H_

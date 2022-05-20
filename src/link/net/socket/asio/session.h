/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_ASIO_CHANNEL_H_
#define LINK_NET_SOCKET_ASIO_CHANNEL_H_

#include <memory>
#include <string>

#include "link/base/callback/callback.h"
#include "link/third_party/asio/asio/ip/tcp.hpp"

namespace nlink {
namespace net {

class SessionManager;

class Session : public std::enable_shared_from_this<Session> {
 public:
  class Delegate {
    virtual void OnSessionClose() = 0;
  };

  using ConnectCallback = base::Callback<void(bool)>;
  using ReadCallback = base::Callback<void(bool)>;
  using WriteCallback = base::Callback<void(bool)>;

  Session(asio::ip::tcp::socket socket, Delegate* delegate);
  ~Session();

  void Open();
  void Close();

  void Connect(const std::string& address, int32_t port);

 private:
  void DoWrite();
  void DoRead();

  ConnectCallback connect_callback_;
  ReadCallback read_callback_;
  WriteCallback write_callback_;

  asio::ip::tcp::socket socket_;
  Delegate* delegate_;
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_SOCKET_ASIO_CHANNEL_H_

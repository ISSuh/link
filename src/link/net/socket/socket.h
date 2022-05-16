/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_SOCKET_H_
#define LINK_NET_SOCKET_SOCKET_H_

#include "asio/ip/tcp.hpp"

namespace nlink {
namespace net {

class Socket {
 public:
  asio::ip::tcp::socket& Descriptor() const {
    return sockeet_;
  }

 private:
  asio::ip::tcp::socket socket_;
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_SOCKET_SOCKET_H_

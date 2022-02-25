/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_SOCKET_H_
#define LINK_NET_SOCKET_SOCKET_H_

namespace link {
namespace net {

class Socket {
 public:
  virtual void Read() = 0;
  virtual void Write() = 0;
};

}  // namespace net
}  // namespace link

#endif  // LINK_NET_SOCKET_SOCKET_H_

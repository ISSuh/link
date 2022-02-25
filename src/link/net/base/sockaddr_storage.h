/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_BASE_SOCKADDR_STORAGE_H_
#define LINK_NET_BASE_SOCKADDR_STORAGE_H_

#include <sys/socket.h>
#include <sys/types.h>

namespace link {
namespace net {

class SockaddrStorage {
 public:
  SockaddrStorage();
  SockaddrStorage(const SockaddrStorage& other);
  void operator=(const SockaddrStorage& other);

  sockaddr_storage addr_storage;
  socklen_t addr_len;
  sockaddr* const addr;
};

}  // namespace net
}  // namespace link

#endif  // LINK_NET_BASE_SOCKADDR_STORAGE_H_

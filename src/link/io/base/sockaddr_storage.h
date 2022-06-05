/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_BASE_SOCKADDR_STORAGE_H_
#define LINK_NET_BASE_SOCKADDR_STORAGE_H_

#include <sys/socket.h>
#include <sys/types.h>

namespace nlink {
namespace io {

class SockaddrStorage {
 public:
  SockaddrStorage();
  SockaddrStorage(const SockaddrStorage& other);
  void operator=(const SockaddrStorage& other);

  sockaddr_storage addr_storage;
  socklen_t addr_len;
  sockaddr* const addr;
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_NET_BASE_SOCKADDR_STORAGE_H_

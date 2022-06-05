/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/base/sockaddr_storage.h"

#include <algorithm>

namespace nlink {
namespace io {

SockaddrStorage::SockaddrStorage()
  : addr_len(sizeof(addr_storage)),
    addr(reinterpret_cast<struct sockaddr*>(&addr_storage)) {
}

SockaddrStorage::SockaddrStorage(const SockaddrStorage& lhs)
  : addr_len(sizeof(lhs.addr_storage)),
    addr(reinterpret_cast<struct sockaddr*>(&addr_storage)) {
  std::copy(lhs.addr, lhs.addr + addr_len, addr);
}

void SockaddrStorage::operator=(const SockaddrStorage& lhs) {
  if (&lhs == this) {
    return;
  }

  addr_len = lhs.addr_len;
  std::copy(lhs.addr, lhs.addr + addr_len, addr);
}


}  // namespace io
}  // namespace nlink

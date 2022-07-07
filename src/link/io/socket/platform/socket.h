/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_IO_SOCKET_PLATFORM_SOCKET_H_
#define LINK_IO_SOCKET_PLATFORM_SOCKET_H_

#include <memory>

#include "link/io/base/address_family.h"
#include "link/io/socket/platform/socket_handle.h"

namespace nlink {
namespace io {

class Socket {
 public:
  enum class SocketType : int32_t {
    TCP = 1,
    UDP = 2
  };

  Socket();
  virtual ~Socket();

  int32_t Open(AddressFamily address_family, SocketType type);

  int32_t Bind();
  int32_t Listen();
  int32_t Accept();

  int32_t Connect();
  int32_t Close();

  int32_t Read();
  int32_t Write();

  bool IsConnected();

 private:
  std::unique_ptr<SocketHandle> handle_;
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_PLATFORM_SOCKET_H_

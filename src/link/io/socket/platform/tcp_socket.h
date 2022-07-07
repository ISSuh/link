/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_IO_SOCKET_PLATFORM_TCP_SOCKET_H_
#define LINK_IO_SOCKET_PLATFORM_TCP_SOCKET_H_

#include "link/io/socket/platform/socket_handle.h"

namespace nlink {
namespace io {

class TcpSocket {
 public:
  TcpSocket();
  ~TcpSocket();

  int32_t Open();

  int32_t Bind();
  int32_t Listen();
  int32_t Accept();

  int32_t Connect();
  int32_t Close();

  int32_t Read();
  int32_t Write();

  bool IsConnected();

 private:
  SocketHandle handle_;
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_IO_SOCKET_PLATFORM_TCP_SOCKET_H_

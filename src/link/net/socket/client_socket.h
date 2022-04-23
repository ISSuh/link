/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_CLIENT_SOCKEET_H_
#define LINK_NET_SOCKET_CLIENT_SOCKEET_H_

#include "link/base/macro.h"
#include "link/base/callback/callback.h"
#include "link/net/base/ip_endpoint.h"

namespace link {
namespace net {

class ClientSocket {
 public:
  ClientSocket() = default;
  virtual ~ClientSocket() = default;

  virtual int32_t Connect(base::CompletionCallback callback) = 0;
  virtual void Disconnect() = 0;
  virtual bool IsConnected() const = 0;

  virtual int32_t Read() = 0;
  virtual int32_t Write() = 0;

 private:
  DISAALOW_COPY_AND_ASSIGN(ClientSocket)
};

}  // namespace net
}  // namespace link

#endif  // LINK_NET_SOCKET_CLIENT_SOCKEET_H_

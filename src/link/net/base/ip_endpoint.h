/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_BASE_IP_ENDPOINT_H_
#define LINK_NET_BASE_IP_ENDPOINT_H_

#include <sys/socket.h>

#include <string>

#include "link/net/base/ip_address.h"
#include "link/net/base/address_family.h"

namespace link {
namespace net {

class IpEndPoint {
 public:
  IpEndPoint() = default;
  IpEndPoint(const std::string& address, uint16_t port);
  IpEndPoint(const IpAddress& address, uint16_t port);
  IpEndPoint(const IpEndPoint& endpoint);
  ~IpEndPoint();

  const IpAddress& address() const;
  uint16_t port() const;

  AddressFamily GetFamily() const;
  int32_t GetSockAddrFamily() const;

  bool ToSockAddr(
    sockaddr* address, socklen_t* address_length) const;

  bool FromSockAddr(
    sockaddr* address, socklen_t address_length);

  bool empty() const;
  const std::string ToString() const;

  IpEndPoint& operator=(const IpEndPoint& lhs);
  bool operator==(const IpEndPoint& lhs) const;

 private:
  IpAddress address_;
  uint16_t port_;
};

}  // namespace net
}  // namespace link

#endif  // LINK_NET_BASE_IP_ENDPOINT_H_

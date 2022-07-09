/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_BASE_IP_ENDPOINT_H_
#define LINK_NET_BASE_IP_ENDPOINT_H_

#include <sys/socket.h>

#include <string>

#include "link/io/base/ip_address.h"
#include "link/io/base/address_family.h"

namespace nlink {
namespace io {

class IpEndPoint {
 public:
  IpEndPoint() = default;
  IpEndPoint(const std::string& address_str, uint16_t port);
  IpEndPoint(const IpAddress& address, uint16_t port);
  IpEndPoint(const IpEndPoint& endpoint);
  ~IpEndPoint();

  const IpAddress& Address() const;
  uint16_t Port() const;

  AddressFamily GetFamily() const;
  int32_t GetSockAddrFamily() const;

  bool IsAdressDomainName() const;

  bool ToSockAddr(
    sockaddr* address, socklen_t* address_length) const;

  bool FromSockAddr(
    sockaddr* address, socklen_t address_length);

  const std::string Origin() const;
  bool Empty() const;

  IpEndPoint& operator=(const IpEndPoint& lhs);
  bool operator==(const IpEndPoint& lhs) const;

 private:
  IpAddress address_;
  uint16_t port_;
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_NET_BASE_IP_ENDPOINT_H_

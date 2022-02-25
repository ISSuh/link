/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/base/ip_endpoint.h"

#include <stddef.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <memory.h>

#include <algorithm>

#include "link/base/endian.h"
#include "link/base/logging.h"

namespace link {
namespace net {

IpEndPoint::IpEndPoint(const std::string& address, uint16_t port)
  : address_(address), port_(port) {
}

IpEndPoint::IpEndPoint(const IpAddress& address, uint16_t port)
  : address_(address), port_(port) {}

IpEndPoint::IpEndPoint(const IpEndPoint& endpoint) = default;

IpEndPoint::~IpEndPoint() = default;

const IpAddress& IpEndPoint::address() const {
  return address_;
}

uint16_t IpEndPoint::port() const {
  return port_;
}

AddressFamily IpEndPoint::GetFamily() const {
  if (address_.IsIPv4()) {
    return ADDRESS_FAMILY_IPV4;
  } else if (address_.IsIPv6()) {
    return ADDRESS_FAMILY_IPV6;
  } else {
    return ADDRESS_FAMILY_UNSPECIFIED;
  }
}

int32_t IpEndPoint::GetSockAddrFamily() const {
  switch (address_.size()) {
    case IpAddress::kIPv4AddressSize:
      return AF_INET;
    case IpAddress::kIPv6AddressSize:
      return AF_INET6;
    default:
      LOG(ERROR) << __func__ << " - Bad IP address";
      return AF_UNSPEC;
  }
}

bool IpEndPoint::ToSockAddr(
  sockaddr* address, socklen_t* address_length) const {
  constexpr socklen_t kSockaddrInSize =
      static_cast<socklen_t>(sizeof(sockaddr_in));
  constexpr socklen_t kSockaddrIn6Size =
      static_cast<socklen_t>(sizeof(sockaddr_in6));

  switch (address_.size()) {
    case IpAddress::kIPv4AddressSize: {
      if (*address_length < kSockaddrInSize) {
        return false;
      }

      *address_length = kSockaddrInSize;

      sockaddr_in* addr = reinterpret_cast<sockaddr_in*>(address);
      memset(addr, 0, sizeof(sockaddr_in));

      addr->sin_family = AF_INET;
      addr->sin_port = base::HostToNet16(port_);

      memcpy(&addr->sin_addr, address_.ToString().data(),
             IpAddress::kIPv4AddressSize);
      break;
    }
    case IpAddress::kIPv6AddressSize: {
      if (*address_length < kSockaddrIn6Size) {
        return false;
      }

      *address_length = kSockaddrIn6Size;

      sockaddr_in6* addr6 =
          reinterpret_cast<sockaddr_in6*>(address);
      memset(addr6, 0, sizeof(sockaddr_in6));

      addr6->sin6_family = AF_INET6;
      addr6->sin6_port = base::HostToNet16(port_);

      memcpy(&addr6->sin6_addr, address_.ToString().data(),
             IpAddress::kIPv6AddressSize);
      break;
    }
    default:
      return false;
  }
  return true;
}

bool IpEndPoint::FromSockAddr(
  sockaddr* sock_addr, socklen_t sock_addr_len) {
  switch (sock_addr->sa_family) {
    case AF_INET: {
      if (sock_addr_len < static_cast<socklen_t>(sizeof(sockaddr_in))) {
        return false;
      }

      const sockaddr_in* addr =
          reinterpret_cast<const sockaddr_in*>(sock_addr);

      *this = IpEndPoint(
        IpAddress(inet_ntoa(addr->sin_addr)),
        base::NetToHost16(addr->sin_port));

      return true;
    }
    case AF_INET6: {
      if (sock_addr_len < static_cast<socklen_t>(sizeof(sockaddr_in6))) {
        return false;
      }

      const sockaddr_in6* addr =
          reinterpret_cast<const sockaddr_in6*>(sock_addr);

      std::string address_str("", IpAddress::kIPv6AddressSize);
      inet_ntop(
        AF_INET6, addr,
        &address_str[0], IpAddress::kIPv6AddressSize);

      *this = IpEndPoint(address_str,
          base::NetToHost16(addr->sin6_port));

      return true;
    }
  }
  return false;  // Unrecognized |sa_family|.
}

const std::string IpEndPoint::ToString() const {
  return address_.ToString() + ":" + std::to_string(port_);
}

IpEndPoint& IpEndPoint::operator=(const IpEndPoint& lhs) {
  if (&lhs == this) {
    return *this;
  }

  address_ = lhs.address_;
  port_ = lhs.port_;
  return *this;
}

bool IpEndPoint::operator==(const IpEndPoint& lhs) const {
  return address_ == lhs.address_ && port_ == lhs.port_;
}

}  // namespace net
}  // namespace link


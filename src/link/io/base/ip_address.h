/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_BASE_IP_ADDRESS_H_
#define LINK_NET_BASE_IP_ADDRESS_H_

#include <string>

namespace nlink {
namespace io {

class IpAddress {
 public:
  enum class Type : int32_t {
    INVALID,
    IPv4,
    IPv6,
    DOMAIN_NAME
  };

  enum : size_t {
    kIPv4AddressSize = 4,
    kIPv6AddressSize = 16
  };

  IpAddress();
  IpAddress(const std::string& address, IpAddress::Type type);
  IpAddress(const IpAddress& lhs);
  IpAddress(IpAddress&& lhs);
  ~IpAddress();

  Type AddressType() const;
  bool IsIPv4() const;
  bool IsIPv6() const;
  bool IsZero() const;
  bool IsLoopback() const;

  bool IsDomainName() const;

  const std::string Origin() const;
  size_t Size() const;
  bool Empty() const;

  IpAddress& operator=(const IpAddress& lhs);
  bool operator==(const IpAddress& lhs) const;

 private:
  std::string address_;
  IpAddress::Type type_;
};

IpAddress ParseAddress(const std::string& address_str);

}  // namespace io
}  // namespace nlink

#endif  // LINK_NET_BASE_IP_ADDRESS_H_

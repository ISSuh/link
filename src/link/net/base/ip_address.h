/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_BASE_IP_ADDRESS_H_
#define LINK_NET_BASE_IP_ADDRESS_H_

#include <string>

namespace nlink {
namespace net {

class IpAddress {
 public:
  enum : size_t {
    kIPv4AddressSize = 4,
    kIPv6AddressSize = 16
  };

  IpAddress() = default;
  explicit IpAddress(const std::string& address_str);
  IpAddress(const IpAddress& lhs);
  IpAddress(IpAddress&& lhs);
  ~IpAddress();

  bool IsIPv4() const;
  bool IsIPv6() const;
  bool IsZero() const;
  bool IsLoopback() const;

  size_t size() const;
  bool empty() const;
  const std::string ToString() const;

  IpAddress& operator=(const IpAddress& lhs);
  bool operator==(const IpAddress& lhs) const;

 private:
  std::string address_;
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_BASE_IP_ADDRESS_H_

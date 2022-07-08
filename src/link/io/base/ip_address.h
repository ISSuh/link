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
  enum : size_t {
    kIPv4AddressSize = 4,
    kIPv6AddressSize = 16
  };

  IpAddress() = default;
  explicit IpAddress(const std::string& address_str);
  IpAddress(const std::string& address_str, bool is_domain_name);
  IpAddress(const IpAddress& lhs);
  IpAddress(IpAddress&& lhs);
  ~IpAddress();

  bool IsIPv4() const;
  bool IsIPv6() const;
  bool IsZero() const;
  bool IsLoopback() const;
  bool IsDomainName() const;

  size_t size() const;
  bool empty() const;
  const std::string ToString() const;

  IpAddress& operator=(const IpAddress& lhs);
  bool operator==(const IpAddress& lhs) const;

 private:
  std::string address_;
  bool is_domain_name_;
};

}  // namespace io
}  // namespace nlink

#endif  // LINK_NET_BASE_IP_ADDRESS_H_

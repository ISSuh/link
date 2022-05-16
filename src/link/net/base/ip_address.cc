/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/base/ip_address.h"

namespace nlink {
namespace net {

IpAddress::IpAddress(const std::string& address)
  : address_(address) {
}

IpAddress::IpAddress(const IpAddress& lhs)
  : address_(lhs.address_) {
}

IpAddress::IpAddress(IpAddress&& lhs)
  : address_(lhs.address_) {
}

IpAddress::~IpAddress() = default;

bool IpAddress::IsIPv4() const {
  return true;
}

bool IpAddress::IsIPv6() const {
  return false;
}

bool IpAddress::IsZero() const {
  return address_ == "0.0.0.0";
}

bool IpAddress::IsLoopback() const {
  return address_ == "127.0.0.1";
}

size_t IpAddress::size() const {
  return IpAddress::kIPv4AddressSize;
}

bool IpAddress::empty() const {
  return address_.empty();
}

const std::string IpAddress::ToString() const {
  return address_;
}

IpAddress& IpAddress::operator=(const IpAddress& lhs) {
  if (&lhs == this) {
    return *this;
  }

  address_ = lhs.address_;
  return *this;
}

bool IpAddress::operator==(const IpAddress& lhs) const {
  return address_ == lhs.address_;
}

}  // namespace net
}  // namespace nlink

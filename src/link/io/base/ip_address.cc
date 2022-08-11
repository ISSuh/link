/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/base/ip_address.h"

#include <arpa/inet.h>
#include <netdb.h>

#include <regex>

#include "link/base/logging.h"

namespace nlink {
namespace io {

bool CheckPattern(const std::string& address, const char* address_pattern) {
  std::regex pattern(address_pattern);
  if (std::regex_match(address, pattern)) {
    return true;
  }
  return false;
}

bool CheckAddressIPv4(const std::string& address) {
  const char* kIPv4Pattern =
      "(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9]["
      "0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])";
  ;

  return CheckPattern(address, kIPv4Pattern);
}

bool CheckAddressIPv6(const std::string& address) {
  const char* kIPv6Pattern =
      "(([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-"
      "9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-"
      "fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-"
      "9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:["
      "0-9a-fA-F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:["
      "0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|:"
      ":(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){"
      "3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:(("
      "25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}(25[0-5]|(2[0-4]|1{0,1}["
      "0-9]){0,1}[0-9]))";

  return CheckPattern(address, kIPv6Pattern);
}

bool IsValidIPv4Address(const std::string& address) {
  struct sockaddr_in sa;
  int result = inet_pton(AF_INET, address.c_str(), &(sa.sin_addr));
  return result != 0;
}

bool ParseDomainName(const std::string& address_str, std::string* ip_str) {
  hostent* he;
  in_addr** addr_list;
  int i;
  if ((he = gethostbyname(address_str.c_str())) == NULL) {
    return false;
  }

  addr_list = (struct in_addr**)he->h_addr_list;
  for (i = 0; addr_list[i] != NULL; i++) {
    *ip_str = inet_ntoa(*addr_list[i]);
    return true;
  }
  return false;
}

IpAddress ParseAddress(const std::string& address_str) {
  IpAddress::Type type = IpAddress::Type::INVALID;
  if (CheckAddressIPv4(address_str)) {
    type = IpAddress::Type::IPv4;
  } else if (CheckAddressIPv6(address_str)) {
    type = IpAddress::Type::IPv6;
  } else {
    std::string ip_str;
    if (ParseDomainName(address_str, &ip_str)) {
      return ParseAddress(ip_str);
    }
  }

  LOG(INFO) << __func__ << " - addr : " << address_str;
  return IpAddress(address_str, type);
}

IpAddress::IpAddress() : IpAddress("", IpAddress::Type::INVALID) {}

IpAddress::IpAddress(const std::string& address, IpAddress::Type type)
    : address_(address), type_(type) {}

IpAddress::IpAddress(const IpAddress& lhs)
    : address_(lhs.address_), type_(lhs.type_) {}

IpAddress::IpAddress(IpAddress&& lhs)
    : address_(lhs.address_), type_(lhs.type_) {}

IpAddress::~IpAddress() = default;

IpAddress::Type IpAddress::AddressType() const {
  return type_;
}

bool IpAddress::IsIPv4() const {
  return type_ == IpAddress::Type::IPv4;
}

bool IpAddress::IsIPv6() const {
  return type_ == IpAddress::Type::IPv6;
}

bool IpAddress::IsZero() const {
  return address_ == "0.0.0.0";
}

bool IpAddress::IsLoopback() const {
  return address_ == "127.0.0.1";
}

bool IpAddress::IsDomainName() const {
  return type_ == IpAddress::Type::DOMAIN_NAME;
}

size_t IpAddress::Size() const {
  switch (type_) {
    case IpAddress::Type::IPv4:
      return IpAddress::kIPv4AddressSize;
    case IpAddress::Type::IPv6:
      return IpAddress::kIPv6AddressSize;
    case IpAddress::Type::DOMAIN_NAME:
      return address_.size();
    default:
      return 0;
  }
}

bool IpAddress::Empty() const {
  return address_.empty();
}

const std::string IpAddress::Origin() const {
  return address_;
}

IpAddress& IpAddress::operator=(const IpAddress& lhs) {
  if (&lhs == this) {
    return *this;
  }

  address_ = lhs.address_;
  type_ = lhs.type_;
  return *this;
}

bool IpAddress::operator==(const IpAddress& lhs) const {
  return address_ == lhs.address_;
}

}  // namespace io
}  // namespace nlink

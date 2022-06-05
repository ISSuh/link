/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_BASE_ADDRESS_FAMILY_H_
#define LINK_NET_BASE_ADDRESS_FAMILY_H_

#include <stdint.h>

namespace nlink {
namespace io {

enum AddressFamily : int32_t {
  ADDRESS_FAMILY_UNSPECIFIED = 0,   // AF_UNSPEC
  ADDRESS_FAMILY_IPV4 = 2,          // AF_INET
  ADDRESS_FAMILY_IPV6 = 10,         // AF_INET6
};

int32_t ConvertAddressFamilyToPlatfrom(AddressFamily address_family);

}  // namespace io
}  // namespace nlink

#endif  // LINK_NET_BASE_ADDRESS_FAMILY_H_

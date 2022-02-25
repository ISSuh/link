/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_BASE_ADDRESS_FAMILY_H_
#define LINK_NET_BASE_ADDRESS_FAMILY_H_

#include <vector>

namespace link {
namespace net {

enum AddressFamily {
  ADDRESS_FAMILY_UNSPECIFIED = 0,   // AF_UNSPEC
  ADDRESS_FAMILY_IPV4 = 2,          // AF_INET
  ADDRESS_FAMILY_IPV6 = 10,         // AF_INET6
};

}  // namespace net
}  // namespace link

#endif  // LINK_NET_BASE_ADDRESS_FAMILY_H_

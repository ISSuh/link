/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/io/base/address_family.h"

#include <sys/socket.h>

namespace nlink {
namespace io {

int32_t ConvertAddressFamilyToPlatfrom(AddressFamily address_family) {
  switch (address_family) {
    case ADDRESS_FAMILY_UNSPECIFIED:
      return AF_UNSPEC;
    case ADDRESS_FAMILY_IPV4:
      return AF_INET;
    case ADDRESS_FAMILY_IPV6:
      return AF_INET6;
  }
  return AF_UNSPEC;
}


}  // namespace io
}  // namespace nlink

/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

namespace link {
namespace module {

#define DISAALOW_COPY(Typename)       \
  Typename(const Typename&) = delete; \
  Typename(Typename&&) = delete;

#define DISAALOW_ASSIGN(Typename)                 \
  Typename& operator=(const Typename&) = delete;

#define DISAALOW_COPY_AND_ASSIGN(Typename)  \
  DISAALOW_COPY(Typename)                   \
  DISAALOW_ASSIGN(Typename)

}  // namespace module
}  // namespace link

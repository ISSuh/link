/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_MACRO_H_
#define LINK_BASE_MACRO_H_

namespace nlink {

#define DISAALOW_COPY(Typename)         \
  Typename(const Typename&) = delete;   \
  Typename(Typename&&) = delete;

#define DISAALOW_ASSIGN(Typename)   \
  Typename& operator=(const Typename&) = delete;

#define DISAALOW_COPY_AND_ASSIGN(Typename)    \
  DISAALOW_COPY(Typename)                     \
  DISAALOW_ASSIGN(Typename)

}  // namespace nlink

#endif  // LINK_BASE_MACRO_H_

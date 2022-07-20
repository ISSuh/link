/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_RPC_RPC_COMPONENT_H_
#define LINK_COMPONENT_RPC_RPC_COMPONENT_H_

#include <string>

#include "link/base/macro.h"
#include "link/component/base/component.h"

namespace nlink {
namespace component {

class RpcComponent : public LinkComponent {
 protected:
  RpcComponent() = default;
  virtual ~RpcComponent() = default;

  DISAALOW_COPY_AND_ASSIGN(RpcComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_RPC_RPC_COMPONENT_H_

/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_RPC_RPC_SERVER_COMPONENT_H_
#define LINK_COMPONENT_RPC_RPC_SERVER_COMPONENT_H_

#include <string>
#include <memory>

#include "link/base/macro.h"
#include "link/base/event/event_util.h"
#include "link/component/rpc/rpc_component.h"
#include "link/base/logging.h"

namespace nlink {
namespace component {

class RpcComponent;

class RpcServerComponent : public RpcComponent {
 public:
  void RegistToController(base::DispatcherConext* dispatcher_context) {
    
  }

  void Open() {}
  void Close() {}
  void RegistService() {}

  void Run() {}

 private:
  RpcServerComponent(const std::string& name);
  virtual ~RpcServerComponent();

  // base::EventChannel
  void HandleEvent(const base::Event& event) override;
  void CloseChannel() override;

  DISAALOW_COPY_AND_ASSIGN(RpcServerComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_RPC_RPC_SERVER_COMPONENT_H_

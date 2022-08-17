
/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_BASE_COMPONENT_FACTORY_H_
#define LINK_COMPONENT_BASE_COMPONENT_FACTORY_H_

#include <memory>

#include "link/base/task/task_runner.h"
#include "link/base/event/event_channel_controller.h"
#include "link/component/ipc/socket/tcp_client_component.h"
#include "link/component/ipc/socket/tcp_server_component.h"
#include "link/component/http/http_client_component.h"
#include "link/component/http/http_server_component.h"

namespace nlink {
namespace component {

class ComponentFctaory {
 public:
  explicit ComponentFctaory(
    std::shared_ptr<base::ComponentChannelController> channel_controller);
  virtual ~ComponentFctaory();

  TcpClientComponent* CreateTcpClientComponent(
    base::TaskRunner* task_runner, SocketComponent::Handler handlers);

  TcpServerComponent* CreateTcpServerComponent(
    base::TaskRunner* task_runner, SocketComponent::Handler handlers);

  template <typename ComponentType>
  ComponentType* CreateHttpComponent(base::TaskRunner* task_runner);

 private:
  std::shared_ptr<base::ComponentChannelController> channel_controller_;
};

template <typename ComponentType>
ComponentType* ComponentFctaory::CreateHttpComponent(
  base::TaskRunner* task_runner) {
  return ComponentType::CreateComponent(channel_controller_.get(), task_runner);
}

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_BASE_COMPONENT_FACTORY_H_


/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/base/component_factory.h"

namespace nlink {
namespace component {

ComponentFctaory::ComponentFctaory(
  std::shared_ptr<base::ComponentChannelController> channel_controller)
  : channel_controller_(channel_controller) {
}

ComponentFctaory::~ComponentFctaory() = default;

TcpClientComponent* ComponentFctaory::CreateTcpClientComponent(
  base::TaskRunner* task_runner, SocketComponent::Handler handlers) {
  return TcpClientComponent::CreateComponent(
    channel_controller_.get(), task_runner, handlers);
}

TcpServerComponent* ComponentFctaory::CreateTcpServerComponent(
  base::TaskRunner* task_runner, SocketComponent::Handler handlers) {
  return TcpServerComponent::CreateComponent(
    channel_controller_.get(), task_runner, handlers);
}

template <typename ComponentType>
ComponentType* ComponentFctaory::CreateHttpComponent(
  base::TaskRunner* task_runner) {
  return ComponentType::CreateComponent(channel_controller_.get(), task_runner);
}

}  // namespace component
}  // namespace nlink

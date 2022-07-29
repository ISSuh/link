
/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/base/component_factory.h"

namespace nlink {
namespace component {

ComponentFctaory::ComponentFctaory(
  std::shared_ptr<base::EventChannelController> channel_controller)
  : channel_controller_(channel_controller) {
}

ComponentFctaory::~ComponentFctaory() {
}

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

HttpClientComponent* ComponentFctaory::CreateHttpClientComponent(
  base::TaskRunner* task_runner) {
  return HttpClientComponent::CreateComponent(
    channel_controller_.get(), task_runner);
}

}  // namespace component
}  // namespace nlink

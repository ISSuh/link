/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/http/http_component.h"

namespace nlink {
namespace component {

HttpComponent::HttpComponent(
  base::ComponentChannelController* channel_controller)
  : LinkComponent(channel_controller) {
}

HttpComponent::~HttpComponent() {
}

}  // namespace component
}  // namespace nlink

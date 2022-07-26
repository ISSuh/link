/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/http/http_server_component.h"

#include <vector>

#include "link/base/logging.h"

namespace nlink {
namespace component {

HttpServerComponent::HttpServerComponent(
  base::EventChannelController* channel_controller)
  : HttpComponent(channel_controller) {
}

HttpServerComponent::~HttpServerComponent() {
}

}  // namespace component
}  // namespace nlink

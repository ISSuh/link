/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/http/http_component.h"

namespace nlink {
namespace component {

HttpComponent::HttpComponent(base::EventChannelObserver* channel_subject)
  : LinkComponent(channel_subject) {
}

HttpComponent::~HttpComponent() {
}

}  // namespace component
}  // namespace nlink

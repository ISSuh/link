/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_BASE_COMPONENT_H_
#define LINK_COMPONENT_BASE_COMPONENT_H_

#include <string>

#include "link/base/macro.h"
#include "link/base/event/event_channel_controller.h"

namespace link {
namespace component {

class LinkComponent {
 protected:
  const std::string ComponentName() const {
    return component_name_;
  }

  base::EventChannelController* EventConrolloer() const {
    return event_controller_;
  }

  explicit LinkComponent(
    const std::string& component_name,
    base::EventChannelController* event_controller)
    : component_name_(component_name), event_controller_(event_controller) {}

  virtual ~LinkComponent() = default;

  std::string component_name_;
  base::EventChannelController* event_controller_;

  DISAALOW_COPY_AND_ASSIGN(LinkComponent);
};

}  // namespace component
}  // namespace link

#endif  // LINK_COMPONENT_BASE_COMPONENT_H_

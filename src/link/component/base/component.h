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
#include "link/base/event/event_dispatcher.h"

namespace nlink {
namespace component {

class LinkComponent {
 public:
  virtual base::EventChannel* GetEventChannel() = 0;

 protected:
  LinkComponent() = default;
  virtual ~LinkComponent() = default;

 private:
  DISAALOW_COPY_AND_ASSIGN(LinkComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_BASE_COMPONENT_H_

/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_BASE_COMPONENT_H_
#define LINK_COMPONENT_BASE_COMPONENT_H_

#include <string>
#include <vector>

#include "link/base/macro.h"
#include "link/base/event/event_channel_controller.h"
#include "link/base/event/event_dispatcher.h"

namespace nlink {
namespace component {

class LinkComponent {
 protected:
  explicit LinkComponent(base::EventChannelObserver* channel_subject);
  virtual ~LinkComponent();

  void AttachChannelsToObserver(base::EventChannel* event_channel);
  void DetatchCahnnelFromObserver(base::EventChannel* event_channel);

 private:
  base::EventChannelObserver* channel_subject_;

  DISAALOW_COPY_AND_ASSIGN(LinkComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_BASE_COMPONENT_H_

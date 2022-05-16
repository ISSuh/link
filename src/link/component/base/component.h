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

class LinkComponent : public base::EventChannel {
 public:
  const std::string ComponentName() const {
    return component_name_;
  }

  virtual void RegistToController(
    base::DispatcherConext* dispatcher_context) = 0;

 protected:
  explicit LinkComponent(const std::string& component_name)
    : component_name_(component_name), dispatcher_context(nullptr) {}

  virtual ~LinkComponent() = default;

 private:
  std::string component_name_;
  base::DispatcherConext* dispatcher_context;

  DISAALOW_COPY_AND_ASSIGN(LinkComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_BASE_COMPONENT_H_

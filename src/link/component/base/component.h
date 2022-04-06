/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_BASE_COMPONENT_H_
#define LINK_COMPONENT_BASE_COMPONENT_H_

#include <string>

#include "link/base/macro.h"

namespace link {
namespace component {

class LinkComponent {
 public:

 private:
  explicit LinkComponent(const std::string& component_name)
    : component_name_(component_name) {}

  virtual ~LinkComponent() = default;

  std::string component_name_;

  DISAALOW_COPY_AND_ASSIGN(LinkComponent);
};

}  // namespace component
}  // namespace link

#endif  // LINK_COMPONENT_BASE_COMPONENT_H_

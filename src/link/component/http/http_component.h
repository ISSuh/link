/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_HTTP_HTTP_COMPONENT_H_
#define LINK_COMPONENT_HTTP_HTTP_COMPONENT_H_

#include "link/component/base/component.h"

namespace nlink {
namespace component {

class HttpComponent : public LinkComponent {
 protected:
  explicit HttpComponent(base::EventChannelController* channel_controller);
  virtual ~HttpComponent();

  DISAALOW_COPY_AND_ASSIGN(HttpComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_HTTP_HTTP_COMPONENT_H_

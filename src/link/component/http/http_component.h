/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_HTTP_HTTP_COMPONENT_H_
#define LINK_COMPONENT_HTTP_HTTP_COMPONENT_H_

#include <functional>

#include "link/component/base/component.h"
#include "link/net/http/header.h"
#include "link/net/http/request.h"
#include "link/net/http/response.h"

namespace nlink {
namespace component {

class HttpComponent : public LinkComponent {
 protected:
  explicit HttpComponent(
    base::ComponentChannelController* channel_controller);
  virtual ~HttpComponent();

  DISAALOW_COPY_AND_ASSIGN(HttpComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_HTTP_HTTP_COMPONENT_H_

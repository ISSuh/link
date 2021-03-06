/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_HTTP_HTTP_SERVER_COMPONENT_H_
#define LINK_COMPONENT_HTTP_HTTP_SERVER_COMPONENT_H_

#include "link/component/http/http_component.h"

namespace nlink {
namespace component {

class HttpComponent;

class HttpServerComponent : public HttpComponent {
 public:

 private:
  explicit HttpServerComponent(base::EventChannelObserver* channel_subject);
  virtual ~HttpServerComponent();

  DISAALOW_COPY_AND_ASSIGN(HttpServerComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_HTTP_HTTP_SERVER_COMPONENT_H_

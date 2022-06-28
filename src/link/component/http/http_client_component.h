/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_HTTP_HTTP_CLIENT_COMPONENT_H_
#define LINK_COMPONENT_HTTP_HTTP_CLIENT_COMPONENT_H_

#include "link/component/http/http_component.h"

namespace nlink {
namespace component {

class HttpComponent;

class HttpClientComponent : public HttpComponent {
 public:

 private:
  base::EventChannel* GetEventChannel() override;

  // LinkComponent
  HttpClientComponent();
  virtual ~HttpClientComponent();

  DISAALOW_COPY_AND_ASSIGN(HttpClientComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_HTTP_HTTP_CLIENT_COMPONENT_H_

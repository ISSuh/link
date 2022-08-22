/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_HTTP_SERVER_HTTP_SESSION_H_
#define LINK_COMPONENT_HTTP_SERVER_HTTP_SESSION_H_

#include "link/base/macro.h"
#include "link/base/task/task_runner.h"
#include "link/net/http/header.h"
#include "link/net/http/request.h"
#include "link/net/http/response.h"

namespace nlink {
namespace component {

class HttpSession {
 protected:
  explicit HttpSession(base::TaskRunner* task_runner);
  virtual ~HttpSession();

 private:
  base::TaskRunner* task_runner_;

  DISAALOW_COPY_AND_ASSIGN(HttpSession);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_HTTP_SERVER_HTTP_SESSION_H_

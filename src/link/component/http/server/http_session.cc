/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/http/server/http_session.h"

namespace nlink {
namespace component {

HttpSession::HttpSession(base::TaskRunner* task_runner)
  : task_runner_(task_runner) {
}

HttpSession::~HttpSession() = default;

}  // namespace component
}  // namespace nlink

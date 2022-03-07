/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NODE_NODE_HANDLE_H_
#define LINK_NODE_NODE_HANDLE_H_

#include <memory>

#include "link/base/event/event_dispatcher.h"

namespace link {
namespace node {

class LinkNodeHandle {
 public:
  LinkNodeHandle();
  ~LinkNodeHandle();

  bool RegistComponent();
  void RunOnce();

 private:
  std::unique_ptr<base::EventDispatcher> event_dispatcher_;
};

}  // namespace node
}  // namespace link

#endif  // LINK_NODE_NODE_HANDLE_H_

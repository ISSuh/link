/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_HANDLE_LINK_HANDLE_H_
#define LINK_HANDLE_LINK_HANDLE_H_

#include <vector>
#include <memory>

#include "link/base/event/event_dispatcher.h"
#include "link/component/base/component.h"

namespace nlink {
namespace handle {

class LinkHandle {
 public:
  LinkHandle();
  ~LinkHandle();

  void Initialize();
  void Run();
  void RunOnce();
  void Shutdown();

  void RegistComponent(component::LinkComponent* component);
  void RegistEventChannls(base::EventChannel* channel);

 private:
  std::unique_ptr<base::EventDispatcher> event_dispatcher_;
};

}  // namespace handle
}  // namespace nlink

#endif  // LINK_HANDLE_LINK_HANDLE_H_

/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_HANDLE_LINK_HANDLE_H_
#define LINK_HANDLE_LINK_HANDLE_H_

#include <memory>

#include "link/base/event/event_dispatcher.h"
#include "link/base/event/event_observer.h"

namespace nlink {
namespace handle {

class LinkHandle {
 public:
  LinkHandle();
  ~LinkHandle();

  void Initialize();
  void RunOnce();
  void Shutdown();

  bool RegistComponent();

  // temporary
  bool RegistEventObserver(base::EventObserver* observer);

 private:
  std::unique_ptr<base::EventDispatcher> event_dispatcher_;
};

}  // namespace handle
}  // namespace nlink

#endif  // LINK_HANDLE_LINK_HANDLE_H_

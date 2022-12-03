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
#include "link/base/event/channel_controller.h"
#include "link/module/base/user_module.h"
// #include "link/component/base/component.h"
// #include "link/component/base/component_factory.h"

namespace nlink {
namespace handle {

class LinkHandle {
 public:
  LinkHandle();
  explicit LinkHandle(nlink::module::UserModule* user_module);
  ~LinkHandle();

  void Initialize();
  void Run();
  void RunOnce();
  void Shutdown();

  // std::weak_ptr<component::ComponentFctaory> ComponentFactory() const;

 private:
  std::unique_ptr<base::EventDispatcher> event_dispatcher_;
  std::shared_ptr<base::ChannelController> channel_controller_;
  // std::shared_ptr<component::ComponentFctaory> component_factory_;
};

}  // namespace handle
}  // namespace nlink

#endif  // LINK_HANDLE_LINK_HANDLE_H_

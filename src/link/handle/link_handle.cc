/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/handle/link_handle.h"

#include "link/base/event/event_channel.h"
#include "link/base/event/event_dispatcher_factory.h"
#include "link/base/logging.h"

namespace nlink {
namespace handle {

LinkHandle::LinkHandle()
  : event_dispatcher_(nullptr),
    channel_controller_(std::make_shared<base::ChannelController>())
    // component_factory_(
    //   std::make_shared<component::ComponentFctaory>(channel_controller_))
{
}

LinkHandle::LinkHandle(nlink::module::UserModule*)
  : event_dispatcher_(nullptr),
    channel_controller_(std::make_shared<base::ChannelController>())
    // component_factory_(
    //   std::make_shared<component::ComponentFctaory>(channel_controller_))
{
}

LinkHandle::~LinkHandle() = default;

void LinkHandle::Initialize() {
  base::EventDispatcher* dispatcher =
    base::EventDispatcherFactory::CreateEventDispatcher();
  if (!dispatcher) {
    return;
  }

  event_dispatcher_.reset(dispatcher);
  event_dispatcher_->RegistEventChannelContoller(channel_controller_);
}

void LinkHandle::Run() {
  if (!event_dispatcher_) {
    return;
  }
  event_dispatcher_->Dispatch();
}

void LinkHandle::RunOnce() {
  if (!event_dispatcher_) {
    return;
  }
  event_dispatcher_->DispatchOnce();
}

void LinkHandle::Shutdown() {
  event_dispatcher_->Stop();
  channel_controller_->CloseAllChannels();
}

// std::weak_ptr<component::ComponentFctaory>
// LinkHandle::ComponentFactory() const {
//   return component_factory_;
// }

}  // namespace handle
}  // namespace nlink

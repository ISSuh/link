/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_COMPONENT_IPC_SOCKET_SOCKET_COMPONENT_H_
#define LINK_COMPONENT_IPC_SOCKET_SOCKET_COMPONENT_H_

#include <string>
#include <memory>
#include <functional>

#include "link/base/macro.h"
#include "link/component/base/component.h"
#include "link/io/socket/session.h"

namespace nlink {
namespace component {

class SocketComponent : public LinkComponent {
 public:
  class Handler {
   public:
    using AcceptHandler = std::function<void(std::shared_ptr<io::Session>)>;
    using ConnectHandler = std::function<void(std::shared_ptr<io::Session>)>;
    using CloseHandler = std::function<void(std::shared_ptr<io::Session>)>;
    using ReadHandler =
      std::function<void(const base::Buffer&, std::shared_ptr<io::Session>)>;
    using WriteHandler = std::function<void(size_t)>;

    AcceptHandler accept_handler;
    ConnectHandler connect_handler;
    CloseHandler close_handler;
    ReadHandler read_handler;
    WriteHandler write_handler;
  };

 protected:
  explicit SocketComponent(
    base::EventChannelController* channel_controller);
  virtual ~SocketComponent();

  DISAALOW_COPY_AND_ASSIGN(SocketComponent);
};

}  // namespace component
}  // namespace nlink

#endif  // LINK_COMPONENT_IPC_SOCKET_SOCKET_COMPONENT_H_

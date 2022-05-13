/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_ASIO_CHANNEL_H_
#define LINK_NET_SOCKET_ASIO_CHANNEL_H_

#include <memory>

#include "link/base/callback/callback.h"

namespace link {
namespace net {

class Channel {
 public:
  using ConnectCallback = base::Callback<void(bool)>;
  using ReadCallback = base::Callback<void(bool)>;
  using WriteCallback = base::Callback<void(bool)>;

  static Channel* Create() {
    return nullptr;
  }

  Channel() {}
  ~Channel() {}

  void Connect() {}
  void Close() {}

  void Write() {}

 private:
  void DoWrite() {}
  void DoRead() {}

  ConnectCallback connect_callback_;
  ReadCallback read_callback_;
  WriteCallback write_callback_;
};

}  // namespace net
}  // namespace link

#endif  // LINK_NET_SOCKET_ASIO_CHANNEL_H_

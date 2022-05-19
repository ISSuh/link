/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_ASIO_TCP_CLIENT_H_
#define LINK_NET_SOCKET_ASIO_TCP_CLIENT_H_

#include <memory>

#include "link/base/macro.h"
#include "link/base/event/event_channel.h"
#include "link/net/socket/client.h"
#include "link/net/socket/asio/session.h"

namespace nlink {
namespace net {

class TcpClient
  : public Client,
    public Session::Delegate {
 public:
  TcpClient();
  virtual ~TcpClient();

  // Client
  void Connect(IpEndPoint endpoint) override;
  void DisConnect() override;
  void Write() override;

  // EventChannel
  void OpenChannel(base::DispatcherConext* context) override;
  void CloseChannel() override;
  void HandleEvent(const base::Event& event) override;

  // Session::Delegate
  void OnSessionClose() override;

 private:
  std::unique_ptr<Session> session_;

  DISAALOW_COPY_AND_ASSIGN(TcpClient)
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_SOCKET_ASIO_TCP_CLIENT_H_

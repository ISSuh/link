/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_ASIO_TCP_CLIENT_H_
#define LINK_NET_SOCKET_ASIO_TCP_CLIENT_H_

#include <memory>
#include <vector>

#include "link/base/macro.h"
#include "link/base/event/event_channel.h"
#include "link/net/socket/client.h"
#include "link/net/socket/asio/connector.h"
#include "link/net/socket/asio/session.h"

namespace nlink {
namespace net {

class TcpClient
  : public Client {
 public:
  TcpClient();
  virtual ~TcpClient();

  // Client
  void Connect(
    IpEndPoint endpoint,
    Client::ConnectHandler connect_handler,
    Client::CloseHandler close_handler) override;
  void DisConnect() override;
  void Write(const base::Buffer& buffer, Client::WriteHandler handler) override;
  void RegistReadHandler(Client::ReadHandler handler) override;

  // EventChannel
  void OpenChannel(base::DispatcherConext* context) override;
  void CloseChannel() override;
  void HandleEvent(const base::Event& event) override;

  // Session::Delegate
  // void OnSessionClose() override;

 private:
  void InternalConnectHandler(std::shared_ptr<ClientSideSession> session);
  // void WriteHandler(std::size_t length);

  std::unique_ptr<Connector> connector_;
  std::shared_ptr<ClientSideSession> session_;

  DISAALOW_COPY_AND_ASSIGN(TcpClient)
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_SOCKET_ASIO_TCP_CLIENT_H_

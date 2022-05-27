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
    handler::ConnectHandler connect_handler,
    handler::CloseHandler close_handler) override;
  void Disconnect() override;
  void Write(const base::Buffer& buffer) override;
  void RegistReadHandler(handler::ReadHandler handler) override;
  void RegistWriteHandler(handler::WriteHandler handler) override;

  // EventChannel
  void OpenChannel(base::DispatcherConext* context) override;
  void CloseChannel() override;
  void HandleEvent(const base::Event& event) override;

  // Session::Delegate
  // void OnSessionClose() override;

 private:
  void InternalConnectHandler(
    std::shared_ptr<ClientSideSession> client_session);
  // void WriteHandler(std::size_t length);

  std::unique_ptr<Connector> connector_;
  std::shared_ptr<ClientSideSession> session_;

  handler::ConnectHandler connect_handler_;
  handler::CloseHandler close_handler_;
  handler::ReadHandler read_handler_;
  handler::WriteHandler write_handler_;

  DISAALOW_COPY_AND_ASSIGN(TcpClient)
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_SOCKET_ASIO_TCP_CLIENT_H_

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
#include "link/net/socket/session.h"
#include "link/net/socket/asio/connector.h"

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
  void RegistIOHandler(
    handler::ReadHandler read_handler,
    handler::WriteHandler write_handler) override;
  void Write(const base::Buffer& buffer) override;

  // EventChannel
  void OpenChannel(base::DispatcherConext* context) override;
  void CloseChannel() override;
  void HandleEvent(const base::Event& event) override;

 private:
  void InternalConnectHandler(std::shared_ptr<Session> session);
  void InternalCloseHandler(std::shared_ptr<Session> session);
  void InternalReadHandler(const base::Buffer& buffer);
  void InternalWriteHandler(size_t length);

  std::unique_ptr<Connector> connector_;
  std::shared_ptr<Session> session_;

  handler::ConnectHandler connect_handler_;
  handler::CloseHandler close_handler_;
  handler::ReadHandler read_handler_;
  handler::WriteHandler write_handler_;

  DISAALOW_COPY_AND_ASSIGN(TcpClient)
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_SOCKET_ASIO_TCP_CLIENT_H_

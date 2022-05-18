/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_ASIO_SESSION_MANAGER_H_
#define LINK_NET_SOCKET_ASIO_SESSION_MANAGER_H_

#include <memory>
#include <set>

#include "link/base/macro.h"
#include "link/net/socket/asio/session.h"
#include "link/third_party/asio/asio/ip/tcp.hpp"

namespace nlink {
namespace net {

class Session;

class SessionManager : public Session::Delegate {
 public:
  SessionManager();
  ~SessionManager();

  void CreateSession(asio::ip::tcp::socket socket);
  void DeleteSession(std::shared_ptr<Session> session);
  void DeleteAllSessions();

  // Session::Delegate
  void OnSessionClose() override;

 private:
  std::set<std::shared_ptr<Session>> sessions_;

  DISAALOW_COPY_AND_ASSIGN(SessionManager)
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_SOCKET_ASIO_SESSION_MANAGER_H_

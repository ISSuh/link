/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_ASIO_SESSION_MANAGER_H_
#define LINK_NET_SOCKET_ASIO_SESSION_MANAGER_H_

#include <memory>
#include <set>

#include "link/net/socket/asio/session.h"
#include "link/third_party/asio/asio/ip/tcp.hpp"

namespace nlink {
namespace net {

class Session;

class SessionManager : public Session::Delegate {
 public:
  class Delegate {
    virtual void OnClose() = 0;
  };

  SessionManager() = default;
  ~SessionManager() = default;

  void CreateSession(asio::ip::tcp::socket socket) {
    auto session = std::make_shared<Session>(socket, this);
    session->Open();

    sessions_.insert(session);
  }

  void DeleteSession() {
  }

  void DeleteAllSessions() {
    for (auto session : sessions_) {
      session->Close();
    }
    sessions_.clear();
  }

  void OnSessionClose() override {}

 private:
  std::set<std::shared_ptr<Session>> sessions_;

  DISAALOW_COPY_AND_ASSIGN(SessionManager)
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_SOCKET_ASIO_SESSION_MANAGER_H_

/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/socket/asio/session_manager.h"

#include <utility>

namespace nlink {
namespace net {

SessionManager::SessionManager() = default;
SessionManager::~SessionManager() = default;

void SessionManager::CreateSession(asio::ip::tcp::socket socket) {
  std::shared_ptr<Session> session =
    std::make_shared<Session>(std::move(socket));
  session->Open();

  sessions_.insert(session);
}

void SessionManager::DeleteSession(std::shared_ptr<Session> session) {
  sessions_.erase(session);
  session->Close();
}

void SessionManager::DeleteAllSessions() {
  for (auto session : sessions_) {
    session->Close();
  }
  sessions_.clear();
}

void SessionManager::OnSessionClose() {
}

}  // namespace net
}  // namespace nlink

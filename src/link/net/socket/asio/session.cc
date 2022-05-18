/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/socket/asio/session.h"

#include <utility>

namespace nlink {
namespace net {

Session::Session(asio::ip::tcp::socket socket, Delegate* delegate)
  : socket_(std::move(socket)),
    delegate_(delegate) {
}

Session::~Session() {}

void Session::Open() {
  DoRead();
}

void Session::Close() {
  socket_.close();
}

void Session::DoWrite() {
}

void Session::DoRead() {
}


}  // namespace net
}  // namespace nlink

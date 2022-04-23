/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_SOCKET_POSIX_POSIX_SOCKET_H_
#define LINK_NET_SOCKET_POSIX_POSIX_SOCKET_H_

#include <memory>

#include "link/net/base/sockaddr_storage.h"
#include "link/net/base/address_family.h"
#include "link/net/base/buffer.h"
#include "link/net/socket/socket_descriptor.h"
#include "link/base/macro.h"
#include "link/base/callback/callback.h"

namespace link {
namespace net {

class PosixSocket {
 public:
  PosixSocket();
  virtual ~PosixSocket();

  int32_t Open(AddressFamily address_family);
  int32_t AdoptConnectedSocket(
    base::Discriptor socket, const SockaddrStorage& peer_address);
  int32_t AdoptUnconnectedSocket(base::Discriptor socket);

  int32_t Bind(const SockaddrStorage& address);
  int32_t Listen(int32_t connection);
  int32_t Accept(
    std::unique_ptr<PosixSocket>* socket,
    base::CompletionCallback callback);

  int32_t AcceptSync(std::unique_ptr<PosixSocket>* socket);

  int32_t Connect(
    const SockaddrStorage& address,
    base::CompletionCallback callback);
  int32_t Close();

  int32_t Read(
    Buffer* buf, int32_t len,
    base::CompletionCallback callback);
  int32_t ReadIfReady(
    Buffer* buf, int32_t len,
    base::CompletionCallback callback);
  int32_t CancelReadIfReady();

  int32_t Write(
    Buffer* buf, int32_t len,
    base::CompletionCallback callback);
  int32_t WaitForWrite(
    Buffer* buf, int32_t len,
    base::CompletionCallback callback);

  bool IsConnected() const;

  int32_t GetPeerAddress(SockaddrStorage* address) const;
  void SetPeerAddress(const SockaddrStorage& address);
  bool HasPeerAddress() const;

  base::Discriptor socket_fd() const { return socket_fd_; }

 private:
  int32_t DoAccept(std::unique_ptr<PosixSocket>* socket);
  void AcceptCompleted();

  int32_t DoConnect();
  void ConnectCompleted();

  int32_t DoRead(Buffer* buf, int32_t len);
  void RetryRead(int32_t res);
  void ReadCompleted();

  int32_t DoWrite(Buffer* buf, int32_t len);
  void WriteCompleted();

  base::Discriptor socket_fd_;
  std::unique_ptr<PosixSocket>* accept_socket_;
  std::unique_ptr<SockaddrStorage> peer_address_;

  bool waiting_connect_ = false;

  std::unique_ptr<Buffer> read_buf_;
  int32_t read_buf_len_;

  std::unique_ptr<Buffer> write_buf_;
  int32_t write_buf_len_;

  base::CompletionCallback accept_callback_;
  base::CompletionCallback read_callback_;
  base::CompletionCallback read_if_ready_callback_;
  base::CompletionCallback write_callback_;

  DISAALOW_COPY_AND_ASSIGN(PosixSocket)
};

}  // namespace net
}  // namespace link

#endif  // LINK_NET_SOCKET_POSIX_POSIX_SOCKET_H_

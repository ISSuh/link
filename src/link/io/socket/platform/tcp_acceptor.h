// /**
//  *
//  *  Copyright:  Copyright (c) 2022, ISSuh
//  *
//  */

// #ifndef SRC_LINK_IO_SOCKET_PLATFORM_TCP_ACCEPTOR_H_
// #define SRC_LINK_IO_SOCKET_PLATFORM_TCP_ACCEPTOR_H_

// #include <memory>
// #include <functional>

// #include "link/base/task/task_runner.h"
// #include "link/io/socket/acceptor.h"
// #include "link/io/socket/platform/tcp_socket.h"

// namespace nlink {
// namespace io {

// class TcpAcceptor : public Acceptor {
//  public:
//   using SocketCreatedCallbak = std::function<void(SocketDescriptor)>;

//   TcpAcceptor(
//     base::TaskRunner* task_runner, SocketCreatedCallbak callback);
//   virtual ~TcpAcceptor();

//   bool Listen(const IpEndPoint& address) override;
//   void Accept(handler::AcceptHandler handler) override;
//   void Close() override;

//  private:
//   void DoAccept();
//   void InternalAcceptHandler(
//     std::unique_ptr<TcpSocket> peer_socket, int32_t res);

//   base::TaskRunner* task_runner_;
//   SocketCreatedCallbak socket_create_callback_;

//   std::unique_ptr<TcpSocket> socket_;

//   handler::AcceptHandler accept_handler_;
// };

// }  // namespace io
// }  // namespace nlink

// #endif  // SRC_LINK_IO_SOCKET_PLATFORM_TCP_ACCEPTOR_H_

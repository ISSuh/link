/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include <string>
#include <memory>
#include <thread>
#include <chrono>

#include <link/base/buffer.h>
#include <link/base/logging.h>
#include <link/base/callback/callback.h>
#include <link/base/callback/bind.h>
#include <link/base/task_manager.h>
#include <link/base/event/event_dispatcher_factory.h>
#include <link/base/event/event_channel.h>
#include <link/io/socket/socket_factory.h>
#include <link/io/base/ip_endpoint.h>
#include <link/io/socket/handler.h>
#include <link/io/socket/session.h>

using namespace nlink;

void TestConnectHandler(std::shared_ptr<io::Session> session) {

}

void TestCloseHandler(std::shared_ptr<io::Session> session) {

}

void TestWriteHandler(size_t size) {

}

void TestReadHandler(
  const base::Buffer& buffer, std::shared_ptr<io::Session> session) {

}

int main(int argc, char* argv[]) {
  base::TaskManager task_manager;
  base::TaskRunner* task_runner = task_manager.CreateTaskRunner(
    "TestWorker", base::TaskRunner::Type::SEQUENCE);

  base::EventDispatcher* dispatcher =
    base::EventDispatcherFactory::CreateEventDispatcher();

  io::Client* client =
    io::SocketFactory::CreateTcpClient(task_runner);

  dispatcher->AttachChannels(
    dynamic_cast<base::EventChannel*>(client));

  io::IpEndPoint endpoint("127.0.0.1", 33669);
  client->Connect(endpoint,
    base::Bind(&TestConnectHandler),
    base::Bind(&TestCloseHandler));

  int32_t count = 0;
  while (count < 5) {
    dispatcher->DispatchOnce();
    // if (!client_.IsConnected()) {
    //   LOG(INFO) << "[ExampleClientModule] wait for connect";
    //   continue;
    // }

    // const uint32_t message_size = 10 * 1024 * 1024;
    // std::string message(message_size, 'a');
    // client_.Write(message);
    // LOG(INFO) << "[ExampleClientModule] write : " << message.size();

    ++count;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

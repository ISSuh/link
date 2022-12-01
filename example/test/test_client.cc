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
#include <link/base/logger.h>
#include <link/base/callback.h>
#include <link/base/callback/bind.h>
#include <link/base/task_manager.h>
#include <link/base/event/event_dispatcher_factory.h>
#include <link/base/event/event_channel.h>
#include <link/io/base/ip_endpoint.h>
#include <link/component/ipc/socket/tcp_client_component.h>

using namespace nlink;

void TestConnectHandler(std::shared_ptr<io::Session> session) {
  LOG(INFO) << __func__;
}

void TestCloseHandler(std::shared_ptr<io::Session> session) {
  LOG(INFO) << __func__;

}

void TestWriteHandler(size_t size) {
  LOG(INFO) << __func__ << " - size : " << size;

}

void TestReadHandler(
  const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
  std::string message = buffer.ToString();
  LOG(INFO) << __func__ << " - message : " << message;
}

int main(int argc, char* argv[]) {
  std::shared_ptr<base::TaskManager> task_manager
    = std::make_shared<base::TaskManager>();
  base::LoggerManager::Instance()->SetTaskManager(task_manager);

  base::TaskRunner* task_runner = task_manager->CreateTaskRunner(
    "TestWorker", base::TaskRunner::Type::SEQUENCE);

  base::EventDispatcher* dispatcher =
    base::EventDispatcherFactory::CreateEventDispatcher();

  component::SocketComponent::Handler handler = {
    component::SocketComponent::Handler::AcceptHandler(),
    [](std::shared_ptr<io::Session> session) { TestConnectHandler(session); },
    [](std::shared_ptr<io::Session> session) { TestCloseHandler(session); },
    [](const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
      TestReadHandler(buffer, session); },
    [](size_t size) { TestWriteHandler(size); }
  };

  auto channel_controller = dispatcher->ChannelController();
  component::TcpClientComponent* client =
    component::TcpClientComponent::CreateComponent(
      channel_controller.get(), task_runner, handler);

  client->Connect("127.0.0.1", 3600);

  int32_t count = 0;
  while (count < 5) {
    dispatcher->DispatchOnce();

    if (!client->IsConnected()) {
      LOG(INFO) << "[ExampleClientModule] wait for connect";
      continue;
    }

    const uint32_t message_size = 10;
    std::string message(message_size, 'a');
    base::Buffer buffer(message);
    client->Write(buffer);

    // const uint32_t message_size = 10 * 1024 * 1024;
    // std::string message(message_size, 'a');
    // base::Buffer buffer(message);
    // client->Write(buffer);
    // LOG(INFO) << "[ExampleClientModule] write : " << message.size();

    ++count;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  client->Disconnect();
  task_manager->StopAllRunner();
  task_manager->WaitForTerminateAllTaskRunner();
}

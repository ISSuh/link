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
#include <link/base/callback/callback.h>
#include <link/base/callback/bind.h>
#include <link/base/task_manager.h>
#include <link/base/event/event_dispatcher_factory.h>
#include <link/base/event/event_channel.h>
#include <link/io/base/ip_endpoint.h>
#include <link/component/ipc/socket/tcp_server_component.h>

using namespace nlink;

void TestAcceptHandler(std::shared_ptr<io::Session> session) {
  LOG(INFO) << __func__;
}

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

  base::EventChannelObserver* channel_subject =
    dynamic_cast<base::EventChannelObserver*>(dispatcher);

  component::SocketComponent::Handler handler = {
    [](std::shared_ptr<io::Session> session) { TestConnectHandler(session); },
    component::SocketComponent::Handler::ConnectHandler(),
    [](std::shared_ptr<io::Session> session) { TestCloseHandler(session); },
    [](const base::Buffer& buffer, std::shared_ptr<io::Session> session) {
      TestReadHandler(buffer, session); },
    [](size_t size) { TestWriteHandler(size); }
  };

  component::TcpServerComponent* server =
    component::TcpServerComponent::CreateComponent(
      channel_subject, task_runner, handler);

  server->Open("127.0.0.1", 3600);

  dispatcher->Dispatch();

  server->Close();
  task_manager->StopAllRunner();
  task_manager->WaitForTerminateAllTaskRunner();
}

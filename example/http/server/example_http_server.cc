/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "example_http_server.h"

#include <vector>

#include "link/base/logging.h"
#include "link/base/json_wrapper.h"

using namespace nlink;

TestHandler::TestHandler()
  : user_id_(0), mock_file_(1024 * 100, 0), read_index_(0) {
}

void TestHandler::HelloWorld(
  const nlink::net::http::Request& request,
  nlink::net::http::Response* response) {
  LOG(INFO) << "TestHandler::HelloWorld - \n" << request.Serialize();

  base::Json test;
  test["id"] = "hello";

  response->SetBody(test.dump(), "application/json");
}

void TestHandler::User(
  const nlink::net::http::Request& request,
  nlink::net::http::Response* response) {
  LOG(INFO) << "TestHandler::User - \n" << request.Serialize();
  response->SetBody("1", "text/plain");
}

void TestHandler::Chunk(
  const nlink::net::http::Request& request,
  nlink::net::http::Response* response) {
  LOG(INFO) << "TestHandler::Chunk - \n" << request.Serialize();
  response->SetHeaderItem("Transfer-Encoding", "chunked");
}

void TestHandler::MockFileRead() {
  if (read_index_ >= mock_file_.size()) {
  }

  uint32_t index = read_index_;
  read_index_ += index;
}

ExampleHttpServer::ExampleHttpServer(
    std::weak_ptr<nlink::base::TaskRunner> task_runner,
    nlink::handle::LinkHandle* handle)
  : server_component_(nullptr) {
  auto component_factory_weak = handle->ComponentFactory();
  auto component_factory = component_factory_weak.lock();

  server_component_ =
    component_factory->CreateHttpComponent<
      nlink::component::HttpServerComponent>(task_runner);
}

ExampleHttpServer::~ExampleHttpServer() = default;

void ExampleHttpServer::OpenServer(const std::string& address, int32_t port) {
  server_component_->Open(address, port);

  RegistRoute();
}

void ExampleHttpServer::CloseServer() {
  server_component_->Close();
}

void ExampleHttpServer::RegistRoute() {
  server_component_->Route(
    "/helloworld",
    [&](const net::http::Request& request, net::http::Response* response) {
      test_handler_.HelloWorld(request, response);
    });

  server_component_->Route(
    "/user/<id>",
    [&](const net::http::Request& request, net::http::Response* response) {
      test_handler_.User(request, response);
    });
}

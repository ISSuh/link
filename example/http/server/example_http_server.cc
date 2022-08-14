/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "example_http_server.h"

#include <vector>

#include "link/base/logging.h"

using namespace nlink;

ExampleHttpServer::ExampleHttpServer(
    nlink::base::TaskRunner* task_runner,
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
}

void ExampleHttpServer::CloseServer() {
  server_component_->Close();
}

void ExampleHttpServer::RegistRoute() {
  server_component_->Route(
    "/user",
    [this](const net::http::Request& request, net::http::Request* response) {
      this->User(request, response);
    });
}

void ExampleHttpServer::User(
  const net::http::Request& request, net::http::Request* response) {
}
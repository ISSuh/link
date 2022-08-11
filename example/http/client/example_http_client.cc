/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "example_http_client.h"

#include <vector>

#include "link/base/logging.h"

using namespace nlink;

ExampleHttpClient::ExampleHttpClient()
  : client_component_(nullptr),
    received_reponse_(false) {
}

ExampleHttpClient::~ExampleHttpClient() = default;

void ExampleHttpClient::CreateAndRegistComponent(
  nlink::base::TaskRunner* task_runner,
  nlink::handle::LinkHandle* handle) {
  auto component_factory_weak = handle->ComponentFactory();
  auto component_factory = component_factory_weak.lock();

  client_component_ =
    component_factory->CreateHttpClientComponent(task_runner);
}

bool ExampleHttpClient::IsReceivedResponse() const {
  return received_reponse_;
}

void ExampleHttpClient::Get(const std::string& path) {
  client_component_->Get(
    path,
    [this](const net::http::Response& response) {
      this->GetHandler(response);
    });

  received_reponse_ = false;
}

void ExampleHttpClient::GetHandler(const net::http::Response& response) {
  LOG(INFO) << "[ExampleHttpClient::GetHandler] \n" << response.Serialize();
  received_reponse_ = true;
}

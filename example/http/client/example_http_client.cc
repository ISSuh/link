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
  client_component_ =
    component::HttpClientComponent::CreateComponent(task_runner);

  handle->RegistComponent(client_component_);
}

bool ExampleHttpClient::IsReceivedResponse() const {
  return received_reponse_;
}

void ExampleHttpClient::Get(const std::string& path) {
  client_component_->Get(
    path,
    base::Bind(&ExampleHttpClient::GetHandler, this));

  received_reponse_ = false;
}

void ExampleHttpClient::GetHandler(const net::http::Response& response) {
  LOG(INFO) << "[ExampleHttpClient::GetHandler] \n" << response.Serialize();
  received_reponse_ = true;
}

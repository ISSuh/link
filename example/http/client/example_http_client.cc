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
    received_get_reponse_(false),
    received_post_reponse_(false) {
}

ExampleHttpClient::~ExampleHttpClient() = default;

void ExampleHttpClient::CreateAndRegistComponent(
  nlink::base::TaskRunner* task_runner,
  nlink::handle::LinkHandle* handle) {
  auto component_factory_weak = handle->ComponentFactory();
  auto component_factory = component_factory_weak.lock();

  client_component_ =
    component_factory->CreateHttpComponent<
      nlink::component::HttpClientComponent>(task_runner);
}

bool ExampleHttpClient::IsReceivedResponse() const {
  return received_get_reponse_ &&
         received_post_reponse_;
}

void ExampleHttpClient::Get(const std::string& path) {
  client_component_->Get(
    path,
    [this](const net::http::Response& response) {
      this->GetHandler(response);
    });
}

void ExampleHttpClient::Post(
  const std::string& path,
  const std::string& content_type,
  const std::string& body) {
  client_component_->Post(
    path,
    content_type,
    body,
    [this](const net::http::Response& response) {
      this->PostHandler(response);
    });
}

void ExampleHttpClient::GetHandler(const net::http::Response& response) {
  auto header = response.Header();
  std::string body = response.Body();

  LOG(INFO) << "[ExampleHttpClient::PostHandler] \n"
            << "header size : " << header.Serialize().size()
            << " / body size : " << body.size()
            << "\n" << response.Serialize();
  received_get_reponse_ = true;
}

void ExampleHttpClient::PostHandler(const net::http::Response& response) {
  auto header = response.Header();
  auto body = response.Body();

  LOG(INFO) << "[ExampleHttpClient::PostHandler] \n"
            << "header size : " << header.Serialize().size()
            << " / body size : " << body.size()
            << "\n" << response.Serialize();

  received_post_reponse_ = true;
}

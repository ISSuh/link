/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/http/http_client_component.h"

#include <utility>

#include "link/base/logging.h"
#include "link/io/socket/socket_factory.h"
#include "link/net/base/uri.h"
#include "link/net/http/parser.h"

namespace nlink {
namespace component {

HttpClientComponent* HttpClientComponent::CreateComponent(
  base::ComponentChannelController* channel_controller,
  std::weak_ptr<base::TaskRunner> task_runner) {
  if (!channel_controller) {
    return nullptr;
  }
  return new HttpClientComponent(channel_controller, task_runner);
}

HttpClientComponent::HttpClientComponent(
  base::ComponentChannelController* channel_controller,
  std::weak_ptr<base::TaskRunner> task_runner)
  : HttpComponent(channel_controller),
    task_runner_(task_runner),
    will_response_chunk_(false) {
}

HttpClientComponent::~HttpClientComponent() = default;

void HttpClientComponent::Get(
  const std::string& uri_string, RequestHandler handler) {
  net::Uri uri = net::Uri::Parse(uri_string);
  if (uri.Empty()) {
    LOG(ERROR) << "[HttpClientComponent::Get] invalid url. "
               << uri.Serialize();
    return;
  }

  net::http::HttpHeader default_header;
  default_header.Set("host", uri.Host());
  default_header.Set("accept", "*/*");
  default_header.Set("user-agent", "nlink/0.0.1");
  DoFetch(net::http::Method::GET, uri, default_header, std::move(handler));
}

void HttpClientComponent::Get(
  const std::string& uri_string,
  const net::http::HttpHeader& header,
  RequestHandler handler) {
  net::Uri uri = net::Uri::Parse(uri_string);
  if (!uri.HasScheme() || !uri.HasHost()) {
    LOG(ERROR) << "[HttpClientComponent::Get] invalid url. "
               << uri.Serialize();
    return;
  }

  DoFetch(net::http::Method::GET, uri, header, std::move(handler));
}

void HttpClientComponent::Post(
  const std::string& uri_string,
  const std::string& content_type,
  const std::string& body,
  RequestHandler handler) {
  net::Uri uri = net::Uri::Parse(uri_string);
  if (uri.Empty()) {
    LOG(ERROR) << "[HttpClientComponent::Post] invalid url. "
               << uri.Serialize();
    return;
  }

  net::http::HttpHeader header;
  header.Set("content-type", content_type);
  header.Set("content-length", body.size());
  DoFetchWithBody(
    net::http::Method::POST, uri, header,
    body, content_type,
    std::move(handler));
}

void HttpClientComponent::Post(
  const std::string& uri_string,
  const net::http::HttpHeader& header,
  const std::string& body,
  RequestHandler handler) {
  net::Uri uri = net::Uri::Parse(uri_string);
  if (uri.Empty()) {
    LOG(ERROR) << "[HttpClientComponent::Post] invalid url. "
               << uri.Serialize();
    return;
  }

  const std::string content_type = header.Find("content-type");
  const std::string content_length = header.Find("content-length");

  if (content_type.empty()) {
    LOG(ERROR) << "[HttpClientComponent::Post] invalid content_type on header. "
               << content_type;
    return;
  }

  if (content_length.empty()) {
    LOG(ERROR) << "[HttpClientComponent::Post] "
               << "invalid content_length on header. "
               << content_length;
    return;
  }

  DoFetchWithBody(
    net::http::Method::POST, uri, header,
    body, content_type,
    std::move(handler));
}

void HttpClientComponent::Put(
  const std::string& uri_string,
  const std::string& content_type,
  const std::string& body,
  RequestHandler handler) {
  net::Uri uri = net::Uri::Parse(uri_string);
  if (uri.Empty()) {
    LOG(ERROR) << "[HttpClientComponent::Put] invalid url. "
               << uri.Serialize();
    return;
  }

  net::http::HttpHeader header;
  header.Set("content-type", content_type);
  header.Set("content-length", body.size());
  DoFetchWithBody(
    net::http::Method::PUT, uri, header,
    body, content_type,
    std::move(handler));
}

void HttpClientComponent::Put(
  const std::string& uri_string,
  const net::http::HttpHeader& header,
  const std::string& body,
  RequestHandler handler) {
  net::Uri uri = net::Uri::Parse(uri_string);
  if (uri.Empty()) {
    LOG(ERROR) << "[HttpClientComponent::Put] invalid url. "
               << uri.Serialize();
    return;
  }

  const std::string content_type = header.Find("content-type");
  const std::string content_length = header.Find("content-length");

  if (content_type.empty()) {
    LOG(ERROR) << "[HttpClientComponent::Put] invalid content_type on header. "
               << content_type;
    return;
  }

  if (content_length.empty()) {
    LOG(ERROR) << "[HttpClientComponent::Put] "
               << "invalid content_length on header. "
               << content_length;
    return;
  }

  DoFetchWithBody(
    net::http::Method::PUT, uri, header,
    body, content_type,
    std::move(handler));
}

void HttpClientComponent::Delete(
  const std::string& uri_string, RequestHandler handler) {
  net::Uri uri = net::Uri::Parse(uri_string);
  if (uri.Empty()) {
    LOG(ERROR) << "[HttpClientComponent::Get] invalid url. "
               << uri.Serialize();
    return;
  }

  net::http::HttpHeader default_header;
  DoFetch(net::http::Method::DELETE, uri, default_header, std::move(handler));
}

void HttpClientComponent::Delete(
  const std::string& uri_string,
  const net::http::HttpHeader& header,
  RequestHandler handler) {
  net::Uri uri = net::Uri::Parse(uri_string);
  if (uri.Empty()) {
    LOG(ERROR) << "[HttpClientComponent::Get] invalid url. "
               << uri.Serialize();
    return;
  }

  DoFetch(net::http::Method::DELETE, uri, header, std::move(handler));
}

void HttpClientComponent::Fetch(
  net::http::Method method,
  const std::string& uri_string,
  RequestHandler handler,
  const net::http::HttpHeader& header,
  const std::string& content_type,
  const std::string& body) {
  net::Uri uri = net::Uri::Parse(uri_string);
  if (uri.Empty()) {
    LOG(ERROR) << "[HttpClientComponent::Fetch] invalid url. "
               << uri.Serialize();
    return;
  }

  switch (method) {
    case net::http::Method::GET:
    case net::http::Method::DELETE:
      DoFetch(method, uri, header, std::move(handler));
      break;
    case net::http::Method::POST:
    case net::http::Method::PUT:
      DoFetchWithBody(
        method, uri, header, body, content_type, std::move(handler));
      break;
    default:
      LOG(WARNING) << "[HttpClientComponent::Fetch] unsupport method. "
                  << MethodToString(method);
      break;
  }
}

void HttpClientComponent::DoFetch(
  net::http::Method method,
  const net::Uri& uri,
  const net::http::HttpHeader& header,
  RequestHandler handler) {
  net::http::Request::RequestLine request_line =
    {method, uri, net::http::Version::HTTP_1_1};
  net::http::Request request(request_line, header);
  if (!request.IsValid()) {
    LOG(ERROR) << "[HttpClientComponent::Get] invalid requese."
               << request.Serialize();
    return;
  }

  CreateIOClientAndConnet(request, std::move(handler));
}

void HttpClientComponent::DoFetchWithBody(
  net::http::Method method,
  const net::Uri& uri,
  const net::http::HttpHeader& header,
  const std::string& body,
  const std::string& content_type,
  RequestHandler handler) {
  if (content_type.empty()) {
    LOG(ERROR) << "[HttpClientComponent::Get] invalid content_type. "
               << content_type;
    return;
  }

  net::http::Request::RequestLine request_line =
    {method, uri, net::http::Version::HTTP_1_1};
  net::http::Request request(request_line, header, body, content_type);
  CreateIOClientAndConnet(request, std::move(handler));
}

void HttpClientComponent::CreateIOClientAndConnet(
  const net::http::Request& request, RequestHandler handler) {
  io::Client* client = io::SocketFactory::CreateTcpClient(task_runner_);
  LinkComponent::AttachChannelsToController(client);

  io::handler::ReadHandler socket_read_handler =
    [this, request_handler = std::move(handler)](
      const base::Buffer& buffer,
      std::shared_ptr<io::Session> session) mutable {
        this->InternalReadHandler(std::move(request_handler), buffer, session);
      };

  io::handler::WriteHandler socket_write_handler =
    [this](size_t length) {
      this->InternalWriteHandler(length);
    };

  client->RegistIOHandler(
    std::move(socket_read_handler), std::move(socket_write_handler));

  auto uri = request.RequestUri();
  client->Connect(
    io::IpEndPoint(uri.Host(), uri.Port()),
    [this, req = std::move(request), client]
    (std::shared_ptr<io::Session> session) {
      this->InternalConnectHandler(req, client, session);
    },
    [this](std::shared_ptr<io::Session> session) {
      this->InternalCloseHandler(session);
    });
}

void HttpClientComponent::InternalConnectHandler(
  const net::http::Request& request,
  io::Client* client,
  std::shared_ptr<io::Session> session) {
  if (session == nullptr) {
    return;
  }

  std::shared_ptr<base::Buffer> buffer =
    std::make_shared<base::Buffer>(request.Serialize());

  client->Write(buffer);

  std::unique_ptr<io::Client> client_ptr;
  client_ptr.reset(client);
  clients_.insert(std::move(client_ptr));
}

void HttpClientComponent::InternalCloseHandler(
  std::shared_ptr<io::Session> session) {
  if (session == nullptr) {
    return;
  }
}

void HttpClientComponent::InternalReadHandler(
  RequestHandler request_handler,
  const base::Buffer& buffer,
  std::shared_ptr<io::Session>) {
  if (buffer.IsEmpty()) {
    return;
  }

  if (will_response_chunk_) {
    net::http::Chunk chunk = net::http::Parser::ParseChunk(buffer);

  } else {
    net::http::Response response = net::http::Parser::ParseResponse(buffer);
    if (!request_handler) {
      return;
    }
    request_handler(response);

    will_response_chunk_ = response.IsChunk();
  }
}

void HttpClientComponent::InternalWriteHandler(size_t) {
}

}  // namespace component
}  // namespace nlink

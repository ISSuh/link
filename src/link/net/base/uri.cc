/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/base/uri.h"

#include "link/base/logging.h"
#include "link/third_party/network/uri.hpp"

namespace nlink {
namespace net {

Uri::UserInfo ParseUserInfo(const std::string& user_info_string) {
  if (user_info_string.empty()) {
    return {};
  }

  const char tocken = ':';
  std::string user_name("");
  std::string user_password("");
  size_t pos = user_info_string.find(tocken);
  if (std::string::npos == pos) {
    user_name = user_info_string;
  } else {
    user_name = user_info_string.substr(0, pos);
    user_password = user_info_string.substr(pos+1, user_info_string.size()-1);
  }
  return {user_name, user_password};
}

std::pair<std::string, uint16_t> SeparateHostAndPort(
  const std::string& host_string) {
  if (host_string.empty()) {
    return {};
  }

  const char tocken = ':';
  std::string host("");
  std::string port_string("");
  size_t pos = host_string.find(tocken);
  if (std::string::npos == pos) {
    host = host_string;
    port_string = "0";
  } else {
    host = host_string.substr(0, pos);
    port_string = host_string.substr(pos+1, host_string.size()-1);
  }

  uint16_t port = 0;
  int32_t temp = std::stoi(port_string);
  if (0 < temp || temp < 65535) {
    port = temp;
  }
  return {host, port};
}

std::vector<Uri::Query> ParseQueries(const std::string& queries_string) {
  std::vector<Uri::Query> queries;
  std::string queries_str = queries_string;
  const char and_tocken = '&';
  const char assign_tocken = '=';

  size_t pos =  queries_str.find(and_tocken, 0);
  while (pos != std::string::npos) {
    std::string key;
    std::string value;
    size_t query_pos = queries_str.find(assign_tocken, pos);
    if (std::string::npos == query_pos) {
      break;
    } else {
      key = queries_str.substr(0, query_pos);
      value = queries_str.substr(query_pos+1, pos);
      queries.emplace_back(key, value);
    }

    queries_str.erase(0, pos + 1);

    pos =  queries_str.find(and_tocken);
  }
  return queries;
}

Uri Uri::Parse(const std::string& uri_string) {
  network::uri parse_uri(uri_string);

  const std::string scheme = parse_uri.scheme().to_string();
  const std::string user_info_string = parse_uri.user_info().to_string();
  Uri::UserInfo user_info = ParseUserInfo(user_info_string);

  const std::string host = parse_uri.host().to_string();
  const std::string port_string = parse_uri.port().to_string();
  int32_t port = std::stoi(port_string);
  if (port <= 0 || 65535 < port) {
    port = 0;
  }

  const std::string path = parse_uri.path().to_string();
  const std::string queries_string = parse_uri.query().to_string();
  std::vector<Uri::Query> queries = ParseQueries(queries_string);

  const std::string fragment = parse_uri.fragment().to_string();

  return Uri(scheme, user_info, host, static_cast<uint16_t>(port),
    path, queries, fragment);
}

Uri::Uri()
  : scheme_(""),
    user_info_({}),
    host_(""),
    port_(0),
    path_(""),
    queries_(),
    fragment_("") {
}

Uri::Uri(
  const std::string& scheme,
  const std::string& host)
  : scheme_(scheme),
  user_info_({}),
  path_(""),
  queries_(),
  fragment_("") {
  auto hostAndPort = SeparateHostAndPort(host);
  host_ = hostAndPort.first;
  port_ = hostAndPort.second;
}

Uri::Uri(
  const std::string& scheme,
  const std::string& host,
  const std::string& path)
  : scheme_(scheme),
    user_info_({}),
    path_(path),
    queries_(),
    fragment_("") {
  auto hostAndPort = SeparateHostAndPort(host);
  host_ = hostAndPort.first;
  port_ = hostAndPort.second;
}

Uri::Uri(const std::string& scheme,
  UserInfo user_info,
  const std::string& host,
  uint16_t port,
  const std::string& path,
  std::vector<Query> queries,
  const std::string& fragment)
  : scheme_(scheme),
    user_info_(user_info),
    host_(host),
    port_(port),
    path_(path),
    queries_(queries),
    fragment_(fragment) {
}

const std::string Uri::Serialize() {
  std::string uri;

  if (!HasScheme()) {
    LOG(WARNING) << "[Uri::Serialize] invalid url. scheme empty";
    return "";
  }
  uri.append(Scheme());
  uri.append("://");

  if (HasUserName()) {
    uri.append(UserName());

    if (HasPassword()) {
      uri.append(":");
      uri.append(Password());
    }
    uri.append("@");
  }

  if (!HasHost()) {
    LOG(WARNING) << "[Uri::Serialize] invalid url. host empty";
    return "";
  }

  uri.append(Host());

  if (HasPort() && PortValidCheck()) {
    uri.append(":");
    uri.append(PortToString());
  }

  uri.append(Path());

  if (HasQuery()) {
    uri.append(MakeQueryString());
  }

  if (HasFragment()) {
    uri.append("#");
    uri.append(Fragment());
  }

  return uri;
}

bool Uri::IsEmpty() const {
  return scheme_.empty() && host_.empty();
}

bool Uri::HasScheme() const {
  return !scheme_.empty();
}

bool Uri::HasUserName() const {
  return !user_info_.name.empty();
}

bool Uri::HasPassword() const {
  return !user_info_.password.empty();
}

bool Uri::HasHost() const {
  return !host_.empty();
}

bool Uri::HasPort() const {
  return port_ <= 0 || 65535 < port_;
}

bool Uri::HasPath() const {
  return !path_.empty();
}

bool Uri::HasQuery() const {
  return !queries_.empty();
}

bool Uri::HasFragment() const {
  return !fragment_.empty();
}

const std::string Uri::Scheme() const {
  return scheme_;
}

const std::string Uri::UserName() const {
  return user_info_.name;
}

const std::string Uri::Password() const {
  return user_info_.password;
}

const std::string Uri::Host() const {
  return host_;
}

uint16_t Uri::Port() const {
  return port_;
}

const std::string Uri::Path() const {
  return path_;
}

const std::string Uri::QueryString() const {
  return MakeQueryString();
}

const std::string Uri::Fragment() const {
  return fragment_;
}

const std::string Uri::PathWithQueryAndFragment() const {
  std::string path_with_query_fragmnet(path_);

  if (!queries_.empty()) {
    path_with_query_fragmnet.append(MakeQueryString());
  }

  if (!fragment_.empty()) {
    path_with_query_fragmnet.append("#");
    path_with_query_fragmnet.append(fragment_);
  }

  return path_with_query_fragmnet;
}

void Uri::PrintForDebug() {
  LOG(INFO) << "[Uri::PrintForDebug] "
            << " scheme_ : " << scheme_ << " / "
            << " username_ : " << user_info_.name << " / "
            << " password_ : " << user_info_.password << " / "
            << " host_ : " << host_ << " / "
            << " port_ : " << port_ << " / "
            << " path_ : " << path_ << " / "
            << " queries_string_ : " << MakeQueryString() << " / "
            << " fragment_ : " << fragment_;
  for (const auto& item : queries_) {
    LOG(INFO) << item.first << " / " << item.second;
  }
}

// bool Uri::ParseScheme(const std::string& uri_string) {
  
// }

// bool Uri::ParseHost(const std::string& uri_string) {

// }

bool Uri::PortValidCheck() const {
  return  0 < port_ && port_ < 65535;
}

const std::string Uri::PortToString() const {
  return std::to_string(port_);
}

const std::string Uri::MakeQueryString() const {
  std::string query_str;
  query_str.append("?");

  auto iter = queries_.begin();
  while (iter != queries_.end()) {
    query_str.append(iter->first);
    query_str.append("=");
    query_str.append(iter->second);

    ++iter;
    if (iter != queries_.end()) {
      query_str.append("&");
    }
  }

  return query_str;
}

}  // namespace net
}  // namespace nlink

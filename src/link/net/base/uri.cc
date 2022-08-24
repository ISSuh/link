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

const uint16_t kInvalidPort = 0;
const uint16_t kDefalutHttpPort = 80;
const uint16_t kDefalutHttpsPort = 443;

const char* kSchemeDelimiter = "://";
const size_t kSchemeDelimiterSize = 3;
const char kCredentialsDelimiter = '@';
const size_t kCredentialsDelimiterSize = 1;
const char kQueryDelimiter = '?';
const size_t kQueryDelimiterSize = 1;
const char kFragmentDelimiter = '#';
const size_t kFragmentDelimiterSize = 1;
const char kColon = ':';
const size_t kColonSize = 1;
const char kSlash = '/';
const size_t kkSlashSize = 1;

std::pair<std::string, std::string> ParseUserInfo(
  const std::string& user_info_string) {
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


  if (!std::all_of(
      port_string.begin(), port_string.end(), ::isdigit)) {
    return {"", 0};
  }

  uint16_t port = 0;
  int32_t temp = std::stoi(port_string);
  if (0 < temp || temp < 65535) {
    port = temp;
  }
  return {host, port};
}

uint16_t UseDefaultPortIfAvaliabe(const std::string& scheme) {
  uint16_t default_port = 0;

  std::string lower_scheme(scheme);
  std::transform(
    lower_scheme.begin(), lower_scheme.end(), lower_scheme.begin(),
      [](unsigned char c){ return std::tolower(c); });

  if ("http" == lower_scheme) {
    default_port = 80u;
  } else if ("http" == lower_scheme) {
    default_port = 443u;
  } else {
    default_port = 0u;
  }
  return default_port;
}

Uri::Authority ParseAuthority(
  const std::string& scheme, const std::string& authority_str) {
  std::string host_str("");
  Uri::Authority authority;
  size_t user_info_pos = authority_str.find(kCredentialsDelimiter);
  if (std::string::npos != user_info_pos) {
    std::string user_info_str = authority_str.substr(0, user_info_pos);
    auto user_info = ParseUserInfo(user_info_str);

    authority.user = user_info.first;
    authority.password = user_info.second;

    host_str =
      authority_str.substr(user_info_pos + kCredentialsDelimiterSize);
  } else {
    host_str = authority_str;
  }

  auto host_with_port = SeparateHostAndPort(host_str);
  authority.host = host_with_port.first;
  authority.port = host_with_port.second;

  return authority;
}

std::vector<Uri::Query> ParseQueries(const std::string& queries_string) {
  std::vector<Uri::Query> queries;
  std::string queries_str = queries_string;
  const char and_tocken = '&';
  const char assign_tocken = '=';

  size_t pos =  queries_str.find(and_tocken, 0);
  if (pos == std::string::npos) {
      std::string key;
      std::string value;
      size_t query_pos = queries_str.find(assign_tocken, 0);
      if (std::string::npos != query_pos) {
        key = queries_str.substr(0, query_pos);
        value = queries_str.substr(query_pos+1, pos);
        queries.emplace_back(key, value);
      }
  } else {
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
  }

  return queries;
}

Uri::Authority::Authority()
  : user(""), password(""), host(""), port(0u) {
}

Uri::Authority::Authority(
  const std::string& user_str,
  const std::string& password_str,
  const std::string& host_str,
  uint16_t port_num)
    : user(user_str), password(password_str), host(host_str), port(port_num) {
}

Uri Uri::Parse(const std::string& uri_string) {
  std::string splited_uri("");
  size_t pos = 0;

  if (uri_string.empty()) {
    return Uri();
  }

  // parse scheme
  size_t scheme_pos = uri_string.find(kSchemeDelimiter);
  if (std::string::npos == scheme_pos) {
    return Uri();
  }

  std::string scheme = uri_string.substr(0, scheme_pos);

  pos = scheme_pos + kSchemeDelimiterSize;
  splited_uri = uri_string.substr(pos);

  if (splited_uri.empty()) {
    return Uri();
  }

  // parse authority
  size_t authority_pos = splited_uri.find(kSlash);
  if (std::string::npos == authority_pos) {
    authority_pos = splited_uri.size();
  }

  std::string authority_str = splited_uri.substr(0, authority_pos);
  Uri::Authority authority = ParseAuthority(scheme, authority_str);
  if (authority.host.empty()) {
    return Uri();
  }

  pos = authority_pos + kkSlashSize;
  if (pos >= splited_uri.size()) {
    return Uri(scheme, authority);
  }

  // parse path
  splited_uri = splited_uri.substr(pos);

  size_t path_pos = splited_uri.find(kQueryDelimiter);
  if (std::string::npos == path_pos) {
    path_pos = splited_uri.size();
  }

  std::string path_str = "/" + splited_uri.substr(0, path_pos);
  pos = path_pos + kQueryDelimiterSize;

  if (pos >= splited_uri.size()) {
    return Uri(scheme, authority, path_str);
  }

  // parse query
  splited_uri = splited_uri.substr(pos);

  size_t query_pos = splited_uri.find(kFragmentDelimiter);
  if (std::string::npos == query_pos) {
    query_pos = splited_uri.size();
  }

  std::string query_str = splited_uri.substr(0, query_pos);
  pos = query_pos + kFragmentDelimiterSize;
  auto query = ParseQueries(query_str);

  // parse fragment;
  if (pos >= splited_uri.size()) {
    return Uri(scheme, authority, path_str, query, "");
  }

  splited_uri = splited_uri.substr(pos);
  std::string fragment_str = splited_uri.substr(0, query_pos);
  return Uri(scheme, authority, path_str, query, fragment_str);
}

Uri::Uri()
  : is_valid_(false),
    scheme_(""),
    authority_({}),
    path_(""),
    queries_(),
    fragment_("") {
}

Uri::Uri(const std::string& scheme,
    const Uri::Authority& authority)
  : is_valid_(true),
    scheme_(scheme),
    authority_(authority),
    path_(""),
    queries_(),
    fragment_("") {
}

Uri::Uri(
  const std::string& scheme,
  const Uri::Authority& authority,
  const std::string& path)
  : is_valid_(true),
    scheme_(scheme),
    authority_(authority),
    path_(path),
    queries_(),
    fragment_("") {
}

Uri::Uri(
  const std::string& scheme,
  Uri::Authority authority,
  const std::string& path,
  std::vector<Query> queries,
  const std::string& fragment)
  : is_valid_(true),
    scheme_(scheme),
    authority_(authority),
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

  if (HasPath()) {
    uri.append(Path());

    if (HasQuery()) {
      uri.append("?");
      uri.append(MakeQueryString());
    }

    if (HasFragment()) {
      uri.append("#");
      uri.append(Fragment());
    }
  }

  return uri;
}

bool Uri::IsEmpty() const {
  return !is_valid_ ||
          (scheme_.empty() && authority_.host.empty());
}

bool Uri::HasScheme() const {
  return is_valid_ && !scheme_.empty();
}

bool Uri::HasUserName() const {
  return is_valid_ && !authority_.user.empty();
}

bool Uri::HasPassword() const {
  return is_valid_ && !authority_.password.empty();
}

bool Uri::HasHost() const {
  return is_valid_ && !authority_.host.empty();
}

bool Uri::HasPort() const {
  return  is_valid_ && 0 < authority_.port && authority_.port < 65535;
}

bool Uri::HasPath() const {
  return is_valid_ && !path_.empty();
}

bool Uri::HasQuery() const {
  return is_valid_ && !queries_.empty();
}

bool Uri::HasFragment() const {
  return is_valid_ && !fragment_.empty();
}

const std::string Uri::Scheme() const {
  return is_valid_ ? scheme_ : "";
}

const std::string Uri::UserName() const {
  return is_valid_ ? authority_.user : "";
}

const std::string Uri::Password() const {
  return is_valid_ ? authority_.password : "";
}

const std::string Uri::Host() const {
  return is_valid_ ? authority_.host : "";
}

uint16_t Uri::Port() const {
  if (!is_valid_) {
    return 0u;
  }

  if (0u == authority_.port) {
    return UseDefaultPortIfAvaliabe(scheme_);
  }
  return authority_.port;
}

const std::string Uri::HostAndPortIfHasPort() const {
  if (!is_valid_) {
    return "";
  }

  if (!HasPort()) {
    return Host();
  }
  return Host() + ":" + std::to_string(Port());
}

const std::string Uri::Path() const {
  return is_valid_ ? path_ : "";
}

const std::string Uri::QueryString() const {
  return is_valid_ ? MakeQueryString() : "";
}

const std::string Uri::Fragment() const {
  return is_valid_ ? fragment_ : "";
}

const std::string Uri::PathWithQueryAndFragment() const {
  if (!is_valid_) {
    return "";
  }

  std::string path_with_query_fragmnet = Path();

  if (!queries_.empty()) {
    path_with_query_fragmnet.append("?");
    path_with_query_fragmnet.append(MakeQueryString());
  }

  if (!fragment_.empty()) {
    path_with_query_fragmnet.append("#");
    path_with_query_fragmnet.append(fragment_);
  }

  return path_with_query_fragmnet;
}

bool Uri::PortValidCheck() const {
  return  0 < authority_.port && authority_.port < 65535;
}

const std::string Uri::PortToString() const {
  return std::to_string(authority_.port);
}

const std::string Uri::MakeQueryString() const {
  std::string query_str;
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

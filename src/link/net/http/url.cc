/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/http/url.h"

#include <regex>

#include "link/base/logging.h"

namespace nlink {
namespace net {
namespace http {

const char* kMatchTarget =
  "(ssh|sftp|ftp|smb|http|https):\\/\\/(?:([^@ ]*)@)?([^:?# ]+)(?::(\\d+))?([^?# ]*)(?:\\?([^# ]*))?(?:#([^ ]*))?";

const char* kQueryTarget =
  "(\\?(([^?;&#=]+=[^?;&#=]+)([;|&]([^?;&#=]+=[^?;&#=]+))*))?";

const char* kSchemeDelm = "://";
const char kUsernameDelm = ':';
const char kPasswordDelm = '@';
const char kPortDelm = ':';
const char kHostDelm = '/';
const char kQueryDelm = '?';
const char kFragmentDelm = '#';

Url::Url() {
}

Url::Url(
  const std::string& scheme,
  const std::string& host,
  const std::string& path)
  : scheme_(scheme),
    username_(""),
    password_(""),
    host_(host),
    port_(""),
    path_(path),
    fragment_("") {
}

Url::Url(const std::string& url_string)
  : scheme_(""),
    username_(""),
    password_(""),
    host_(""),
    port_(""),
    path_(""),
    fragment_("") {
  Decode(url_string);
}

const std::string Url::Encode() {
  std::string url;

  if (scheme_.empty()) {
    return "";
  }
  url.append(scheme_);
  url.append("://");

  if (!username_.empty()) {
    url.append(username_);

    if (!password_.empty()) {
      url.append(":");
      url.append(password_);
    }
    url.append("@");
  }

  if (host_.empty()) {
    return "";
  }
  url.append(host_);

  if (!port_.empty()) {
    url.append(":");
    url.append(port_);
  }

  url.append(path_);

  if (!queries_.empty()) {
    url.append(MakeQueryString(queries_));
  }

  if (!fragment_.empty()) {
    url.append("#");
    url.append(fragment_);
  }

  return url;
}

// bool Url::Decode(const std::string& url_string) {
//   std::regex re(kMatchTarget);
//   std::smatch match;

//   if (std::regex_match(url_string, match, re)) {
//     scheme_ = std::string(match[1].first, match[1].second);

//     std::string user(std::string(match[2].first, match[2].second));
//     ParseUserInfo(user);

//     host_ = std::string(match[3].first, match[3].second);
//     port_ = std::string(match[4].first, match[4].second);
//     path_ = std::string(match[5].first, match[5].second);

//     std::string queries(std::string(match[6].first, match[6].second));
//     ParseQueries(queries);

//     fragment_ = std::string(match[7].first, match[7].second);
//   }
// }

bool Url::Decode(const std::string& url_string) {
  size_t pos = url_string.find(kSchemeDelm);
  if (pos == std::string::npos) {
    LOG(WARNING) << "[Url::Decode] can not fine scheme";
    return false;
  }

  scheme_ = url_string.substr(0, pos);
  pos += sizeof(kSchemeDelm);

  size_t pos = url_string.find();
  
}

bool Url::HasScheme() const {
  return !scheme_.empty();
}

bool Url::HasUserName() const {
  return !username_.empty();
}

bool Url::HasPassword() const {
  return !password_.empty();
}

bool Url::HasHost() const {
  return !host_.empty();
}

bool Url::HasPort() const {
  return !port_.empty();
}

bool Url::HasPath() const {
  return !path_.empty();
}

bool Url::HasQuery() const {
  return queries_string_.empty();
}
bool Url::HasFragment() const {
  return fragment_.empty();
}

const std::string Url::Scheme() const {
  return scheme_;
}

const std::string Url::UserName() const {
  return username_;
}

const std::string Url::Password() const {
  return password_;
}

const std::string Url::Host() const {
  return host_;
}

const std::string Url::Port() const {
  return port_;
}

const std::string Url::Path() const {
  return path_;
}

const std::string Url::Query() const {
  return queries_string_;
}

const std::string Url::Fragment() const {
  return fragment_;
}

const std::string Url::Scheme() const {
  return scheme_;
}

const std::string Url::Host() const {
  if (port_.empty()) {
    return host_;
  }
  return host_ + ":" + port_;
}

const std::string Url::Path() const {
  return path_;
}

const std::string Url::PathWithQueryAndFragment() const {
  std::string path_with_query_fragmnet(path_);

  if (!queries_.empty()) {
    path_with_query_fragmnet.append(MakeQueryString(queries_));
  }

  if (!fragment_.empty()) {
    path_with_query_fragmnet.append("#");
    path_with_query_fragmnet.append(fragment_);
  }

  return path_with_query_fragmnet;
}

void Url::PrintForDebug() {
  LOG(INFO) << "[Url::PrintForDebug] "
            << " scheme_ : " << scheme_
            << " username_ : " << username_
            << " password_ : " << password_
            << " host_ : " << host_
            << " port_ : " << port_
            << " path_ : " << path_
            << " fragment_ : " << fragment_;\
  for (const auto& item : queries_) {
    LOG(INFO) << item.first << " / " << item.second;
  }
}

void Url::ParseUserInfo(const std::string& user_info_string) {
  if (user_info_string.empty()) {
    return;
  }

  size_t pos = user_info_string.find(':');
  if (std::string::npos == pos) {
    username_ = user_info_string;
  } else {
    username_ = user_info_string.substr(0, pos);
    password_ = user_info_string.substr(pos+1, user_info_string.size()-1);
  }
}

void Url::ParseQueries(const std::string& queries_string) {
  std::cout << queries_string << std::endl;
  std::string queries = queries_string;
  const char and_tocken = '&';
  const char assign_tocken = '=';

  size_t pos =  queries.find(and_tocken, 0);
  while (pos != std::string::npos) {
    std::string key;
    std::string value;
    size_t query_pos = queries.find(assign_tocken, pos);
    if (std::string::npos == query_pos) {
      break;
    } else {
      key = queries.substr(0, query_pos);
      value = queries.substr(query_pos+1, pos);
      queries_.emplace_back(key, value);
    }

    std::cout << queries << std::endl;
    queries.erase(0, pos + 1);

    pos =  queries.find(and_tocken);
  }
}

const std::string Url::MakeQueryString(
  const std::vector<Url::Query>& queries) const {
  std::string query_str;
  query_str.append("?");

  auto iter = queries.begin();
  while (iter != queries.end()) {
    query_str.append(iter->first);
    query_str.append("=");
    query_str.append(iter->second);

    ++iter;
    if (iter != queries.end()) {
      query_str.append("&");
    }
  }

  return query_str;
}

}  // namespace http
}  // namespace net
}  // namespace nlink

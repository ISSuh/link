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

const char* kMatchTarget =
  "(ssh|sftp|ftp|smb|http|https):\\/\\/(?:([^@ ]*)@)?([^:?# ]+)(?::(\\d+))?([^?# ]*)(?:\\?([^# ]*))?(?:#([^ ]*))?";

const char* kQueryTarget =
  "(\\?(([^?;&#=]+=[^?;&#=]+)([;|&]([^?;&#=]+=[^?;&#=]+))*))?";

Url::Url() {
}

Url::Url(const std::string& url_string) {
  Decode(url_string);
}

std::string Url::Encode() {
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
    url.append("?");

    auto iter = queries_.begin();
    while (iter != queries_.end()) {
      url.append(iter->first);
      url.append("=");
      url.append(iter->second);

      ++iter;
      if (iter != queries_.end()) {
        url.append("&");
      }
    }

    for (const auto& item : queries_) {
      url.append(item.first);
      url.append("=");
      url.append(item.second);
    }
  }

  if (!fragment_.empty()) {
    url.append("#");
    url.append(fragment_);
  }

  return url;
}
void Url::Decode(const std::string& url_string) {
  std::regex re(kMatchTarget);
  std::smatch match;

  if (std::regex_match(url_string, match, re)) {
    scheme_ = std::string(match[1].first, match[1].second);

    std::string user(std::string(match[2].first, match[2].second));
    ParseUserInfo(user);

    host_ = std::string(match[3].first, match[3].second);
    port_ = std::string(match[4].first, match[4].second);
    path_ = std::string(match[5].first, match[5].second);

    std::string queries(std::string(match[6].first, match[6].second));
    ParseQueries(queries);

    fragment_ = std::string(match[7].first, match[7].second);
  }
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

}  // namespace net
}  // namespace nlink

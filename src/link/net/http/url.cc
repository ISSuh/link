/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/base/url.h"

#include <regex>

#include "link/base/logging.h"

namespace nlink {
namespace net {

const char* kMatchTarget =
  "(ssh|sftp|ftp|smb|http|https):\\/\\/(?:([^@ ]*)@)?([^:?# ]+)(?::(\\d+))?([^?# ]*)(?:\\?([^# ]*))?(?:#([^ ]*))?";

Url::Url() {
}

Url::Url(const std::string& url_string) {
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

  if (0 != port_) {
    url.append(":");
    url.append(std::to_string(port_));
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

  std::regex_match(url_string, match, re);

  for (const auto& item : match) {
    std::string test(item.first, item.second);
    LOG(INFO) << __func__ << " - " << test;
  }
}

std::string Url::PrintForDebug() {
  LOG(INFO) << "[Url::PrintForDebug] "
            << " scheme_ : " << scheme_
            << " username_ : " << username_
            << " password_ : " << password_
            << " host_ : " << host_
            << " port_ : " << port_
            << " path_ : " << path_
            << " fragment_ : " << fragment_;
}

}  // namespace net
}  // namespace nlink

#include "Url.h"

// Url::Url() {
//     this -> ex = boost::regex("(ssh|sftp|ftp|smb|http|https):\\/\\/(?:([^@ ]*)@)?([^:?# ]+)(?::(\\d+))?([^?# ]*)(?:\\?([^# ]*))?(?:#([^ ]*))?");
// }

// Url::Url(string &rawUrl) : Url() {
//     this->rawUrl = rawUrl;
//     this->update(this->rawUrl);
// }

// Url &Url::update(string &rawUrl) {
//     this->rawUrl = rawUrl;
//     boost::cmatch what;
//     if (regex_match(rawUrl.c_str(), what, ex)) {
//         this -> protocol = string(what[1].first, what[1].second);
//         this -> username = string(what[2].first, what[2].second);
//         this -> domain = string(what[3].first, what[3].second);
//         this -> port = string(what[4].first, what[4].second);
//         this -> path = string(what[5].first, what[5].second);
//         this -> query = string(what[6].first, what[6].second);
//         this -> fragment = string(what[7].first, what[7].second);
//     }
//     return *this;
// }
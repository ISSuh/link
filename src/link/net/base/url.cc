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
  "(http|https)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)";

Url::Url() {
}

Url::Url(const std::string& url_string) {
}

std::string Url::Encode() {
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

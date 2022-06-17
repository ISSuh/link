/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/http/request_parser.h"

#include <string>

#include "link/base/logging.h"
#include "link/third_party/picohttpparser/picohttpparser.h"

namespace nlink {
namespace net {
namespace http {

#define REQ                                                                                                                        \
    "GET /wp-content/uploads/2010/03/hello-kitty-darth-vader-pink.jpg HTTP/1.1\r\n"                                                \
    "Host: www.kittyhell.com\r\n"                                                                                                  \
    "User-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.6; ja-JP-mac; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3 "             \
    "Pathtraq/0.9\r\n"                                                                                                             \
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"                                                  \
    "Accept-Language: ja,en-us;q=0.7,en;q=0.3\r\n"                                                                                 \
    "Accept-Encoding: gzip,deflate\r\n"                                                                                            \
    "Accept-Charset: Shift_JIS,utf-8;q=0.7,*;q=0.7\r\n"                                                                            \
    "Keep-Alive: 115\r\n"                                                                                                          \
    "Connection: keep-alive\r\n"                                                                                                   \
    "Cookie: wp_ozh_wsa_visits=2; wp_ozh_wsa_visit_lasttime=xxxxxxxxxx; "                                                          \
    "__utma=xxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.x; "                                                             \
    "__utmz=xxxxxxxxx.xxxxxxxxxx.x.x.utmccn=(referral)|utmcsr=reader.livedoor.com|utmcct=/reader/|utmcmd=referral\r\n"             \
    "\r\n"

const size_t kMaxHeaderNum = 100;
const char* kDelm = "\r\n\r\n";

Request RequestParser::Parse(const base::Buffer& buffer) {
  std::string request_str(buffer.Data().begin(), buffer.Data().end());
  auto pos = request_str.find(kDelm);
  if (pos == std::string::npos) {
    LOG(WARNING) << "[RequestParser::Parse] invalid request format";
    return Request();
  }

  const std::string header_str(REQ);
  //  = request_str.substr(0, pos);
  
  const std::string body_str =
    request_str.substr(pos + 4, request_str.size() - 1);

  const char *method;
  size_t method_len;
  const char *path;
  size_t path_len;
  int minor_version;
  struct phr_header headers[kMaxHeaderNum];
  size_t num_headers = sizeof(headers) / sizeof(headers[0]);;

  int32_t res = phr_parse_request(
    header_str.data(), header_str.size(),
    &method, &method_len,
    &path, &path_len,
    &minor_version, headers, &num_headers, 0);

  if (res == -1) {
    LOG(WARNING) << "[RequestParser::Parse] header parse fail " << res;
    return Request();
  }

  Header header;
  for (size_t i = 0 ; i < num_headers ; ++i) {
    LOG(INFO) << __func__ << " | " << headers[i].name << " / " << headers[i].value;

    header.Set({headers[i].name, headers[i].value});
  }

  Url url(path);
  return Request(method, url, header, body_str);
}

}  // namespace http
}  // namespace net
}  // namespace nlink

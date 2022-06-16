/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_HTTP_URL_H_
#define LINK_NET_HTTP_URL_H_

#include <string>
#include <vector>
#include <utility>

namespace nlink {
namespace net {
namespace http {

class Url {
 public:
  using Query = std::pair<std::string, std::string>;

  Url();
  explicit Url(const std::string& url_string);

  std::string Encode();
  void Decode(const std::string& url_string);

  const std::string Scheme() const;
  const std::string Host() const;
  const std::string Path() const;

  const std::string PathWithQueryAndFragment() const;

  void PrintForDebug();

 private:
  void ParseUserInfo(const std::string& user_info_string);
  void ParseQueries(const std::string& queries_string);
  const std::string MakeQueryString() const;

  std::string scheme_;
  std::string username_;
  std::string password_;
  std::string host_;
  std::string port_;
  std::string path_;
  std::vector<Query> queries_;
  std::string fragment_;
};

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_URL_H_

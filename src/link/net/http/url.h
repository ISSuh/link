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
  Url(const std::string& scheme,
      const std::string& host,
      const std::string& path);
  explicit Url(const std::string& url_string);

  const std::string Encode();
  bool Decode(const std::string& url_string);

  bool HasScheme() const;
  bool HasUserName() const;
  bool HasPassword() const;
  bool HasHost() const;
  bool HasPort() const;
  bool HasPath() const;
  bool HasQuery() const;
  bool HasFragment() const;

  const std::string Scheme() const;
  const std::string UserName() const;
  const std::string Password() const;
  const std::string Host() const;
  const std::string Port() const;
  const std::string Path() const;
  const std::string Query() const;
  const std::string Fragment() const;

  const std::string PathWithQueryAndFragment() const;

  void PrintForDebug();

 private:
  void ParseUserInfo(const std::string& user_info_string);
  void ParseQueries(const std::string& queries_string);
  const std::string MakeQueryString(
    const std::vector<Url::Query>& queries) const;

  std::string scheme_;
  std::string username_;
  std::string password_;
  std::string host_;
  std::string port_;
  std::string path_;
  std::vector<Query> queries_;
  std::string queries_string_;
  std::string fragment_;
};

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_URL_H_

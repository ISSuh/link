/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_BASE_URI_H_
#define LINK_NET_BASE_URI_H_

#include <string>
#include <vector>
#include <utility>

namespace nlink {
namespace net {

class Uri {
 public:
  using Query = std::pair<std::string, std::string>;

  struct UserInfo {
    std::string name;
    std::string password;
  };

  static Uri Parse(const std::string& uri_string);

  Uri();
  Uri(const std::string& scheme,
      const std::string& host);
  Uri(const std::string& scheme,
      const std::string& host,
      const std::string& path);
  Uri(const std::string& scheme,
      UserInfo user_info,
      const std::string& host,
      uint16_t port,
      const std::string& path,
      std::vector<Query> queries,
      const std::string& fragment);

  const std::string Serialize();

  bool IsEmpty() const;

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
  const uint16_t Port() const;
  const std::string Path() const;
  const std::string QueryString() const;
  const std::string Fragment() const;

  const std::string PathWithQueryAndFragment() const;

  void PrintForDebug();

 private:
  // bool ParseScheme(const std::string& uri_string);
  // bool ParseHost(const std::string& uri_string);
  // void ParseUserInfo(const std::string& user_info_string);
  // void ParsePath();
  // void ParseFragment();

  bool PortValidCheck() const;
  const std::string PortToString() const;
  const std::string MakeQueryString() const;

  std::string scheme_;
  UserInfo user_info_;
  std::string host_;
  uint16_t port_;
  std::string path_;
  std::vector<Query> queries_;
  std::string fragment_;
};

}  // namespace net
}  // namespace nlink

#endif  // SRC_LINK_NET_BASE_URI_H_

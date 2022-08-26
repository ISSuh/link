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

  struct Authority {
    Authority();
    Authority(
      const std::string& user_str,
      const std::string& password_str,
      const std::string& host_str,
      uint16_t port_num);

    std::string user;
    std::string password;
    std::string host;
    uint16_t port;
  };

  static Uri Parse(const std::string& uri_string);

  Uri();
  Uri(const std::string& scheme,
      const Uri::Authority& authority);
  Uri(const std::string& scheme,
      const Uri::Authority& authority,
      const std::string& path);
  Uri(const std::string& scheme,
      Authority authority,
      const std::string& path,
      std::vector<Query> queries,
      const std::string& fragment);

  const std::string Serialize() const;

  bool Empty() const;

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
  uint16_t Port() const;
  const std::string HostAndPortIfHasPort() const;
  const std::string Path() const;
  const std::string QueryString() const;
  const std::string Fragment() const;

  const std::string PathWithQueryAndFragment() const;

  const std::string QueryParam(const std::string& key) const;

 private:
  bool PortValidCheck() const;
  const std::string PortToString() const;
  const std::string MakeQueryString() const;

  bool is_valid_;
  std::string scheme_;
  Authority authority_;
  std::string path_;
  std::vector<Query> queries_;
  std::string fragment_;
};

}  // namespace net
}  // namespace nlink

#endif  // SRC_LINK_NET_BASE_URI_H_

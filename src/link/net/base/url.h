/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_BASE_URL_H_
#define LINK_NET_BASE_URL_H_

#include <string>
#include <vector>
#include <utility>

namespace nlink {
namespace net {

class Url {
 public:
  using Query = std::pair<std::string, std::string>;

  Url();
  ~Url();

  

 private:
  std::string scheme_;
  std::string user_;
  std::string password_;
  std::string address_;
  std::string port_;
  std::string path_;
  std::vector<Query> qeuries_;
};

}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_BASE_URL_H_

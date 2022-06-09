/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_HTTP_HEADER_H_
#define LINK_NET_HTTP_HEADER_H_

#include <string>
#include <map>

namespace nlink {
namespace net {
namespace http {

class Header {
 public:
  Header();
  ~Header();

  void Insert(const std::string& key, const std::string& value);
  void Insert(const std::string& header);

  std::string Serialize() const;

  std::string ToString() const;

 private:
  std::string version_;
  std::string method_;
  std::string path_;
  std::map<std::string, std::string> header_fields_;
};

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif LINK_NET_HTTP_HEADER_H_

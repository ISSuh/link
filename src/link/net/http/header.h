/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_NET_HTTP_HEADER_H_
#define LINK_NET_HTTP_HEADER_H_

#include <string>
#include <map>
#include <utility>

namespace nlink {
namespace net {
namespace http {

class HttpHeader {
 public:
  HttpHeader();
  ~HttpHeader();

  void Set(const std::pair<std::string, std::string>& header);
  void Set(const std::string& key, const std::string& value);
  bool ParseAndSet(const std::string& header_str);

  const std::string Serialize() const;

  bool Empty() const;
  const std::string Find(const std::string& key) const;

 private:
  std::map<std::string, std::string> header_fields_;
};

}  // namespace http
}  // namespace net
}  // namespace nlink

#endif  // LINK_NET_HTTP_HEADER_H_

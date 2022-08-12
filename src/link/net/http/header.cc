/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/http/header.h"

#include <sstream>

#include "link/base/logging.h"

namespace nlink {
namespace net {
namespace http {

constexpr const char kHeaderDelm = ':';

HttpHeader::HttpHeader()
  : header_fields_({
    {"User-Agent", "nLink   0.0.1"},
    {"Connection", "Close"}}) {
}

HttpHeader::~HttpHeader() = default;

void HttpHeader::Set(const std::pair<std::string, std::string>& header) {
  header_fields_.insert(header);
}

void HttpHeader::Set(const std::string& key, const std::string& value) {
  header_fields_.insert({key, value});
}

bool HttpHeader::ParseAndSet(const std::string& header_str) {
  if (header_str.empty()) {
    return false;
  }

  size_t pos = 0;
  while (pos < header_str.size() &&
         header_str[pos] != kHeaderDelm) {
    ++pos;
  }

  if (pos >= header_str.size()) {
    return false;
  }

  const size_t space_size = 1;
  const std::string key = header_str.substr(0, pos);
  const std::string value =
    header_str.substr(pos + 1 + space_size, header_str.size());

  Set(key, value);
  return true;
}

const std::string HttpHeader::Serialize() const {
  std::stringstream stream;
  for (const auto& header : header_fields_) {
    const std::string& key = header.first;
    const std::string& value = header.second;

    if (key.empty() || value.empty()) {
      continue;
    }

    stream << key;
    stream << ": ";
    stream << value;
    stream << "\r\n";
  }
  return stream.str();
}

bool HttpHeader::Empty() const {
  return header_fields_.empty();
}

const std::string HttpHeader::Find(const std::string& key) const {
  if (header_fields_.find(key) == header_fields_.end()) {
    return "";
  }
  return header_fields_.at(key);
}


}  // namespace http
}  // namespace net
}  // namespace nlink

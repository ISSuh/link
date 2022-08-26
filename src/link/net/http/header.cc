/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/http/header.h"

#include <sstream>
#include <algorithm>

#include "link/base/logging.h"

namespace nlink {
namespace net {
namespace http {

constexpr const char kHeaderDelm = ':';
constexpr const size_t kHeaderDelmSize = 1;
constexpr const char kSpace = ' ';
constexpr const size_t kSpaceSize = 1;

std::string ToLowerKey(const std::string& key) {
  std::string lower_key(key);
  std::transform(
    lower_key.begin(), lower_key.end(), lower_key.begin(), ::tolower);
  return lower_key;
}

HttpHeader::HttpHeader()
  : header_fields_() {
}

HttpHeader::~HttpHeader() = default;

void HttpHeader::Set(const std::string& key, const std::string& value) {
  std::string lower_key = ToLowerKey(key);
  header_fields_[lower_key] = value;
}

void HttpHeader::Set(const std::string& key, int32_t value) {
  std::string lower_key = ToLowerKey(key);
  header_fields_[lower_key] = std::to_string(value);
}

bool HttpHeader::Set(const std::string& header_str) {
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

  if (header_str[pos + 1] != ' ') {
    return false;
  }

  size_t key_pos = pos;
  size_t value_pos = pos + kHeaderDelmSize + kSpaceSize;
  if (value_pos >= header_str.size()) {
    return false;
  }

  const std::string key = header_str.substr(0, key_pos);
  const std::string value =
    header_str.substr(value_pos, header_str.size());

  std::string lower_key = ToLowerKey(key);
  Set(lower_key, value);
  return true;
}

const std::string HttpHeader::Serialize() const {
  const char* CRLF = "\r\n";
  std::stringstream stream;
  for (const auto& header : header_fields_) {
    const std::string& key = header.first;
    const std::string& value = header.second;

    stream << key;
    stream << kHeaderDelm << kSpace;
    stream << value;
    stream << CRLF;
  }
  return stream.str();
}

bool HttpHeader::Empty() const {
  return header_fields_.empty();
}

size_t HttpHeader::Size() const {
  return header_fields_.size();
}

const std::string HttpHeader::Find(const std::string& key) const {
  std::string lower_key = ToLowerKey(key);
  if (header_fields_.find(lower_key) == header_fields_.end()) {
    return "";
  }
  return header_fields_.at(lower_key);
}


}  // namespace http
}  // namespace net
}  // namespace nlink

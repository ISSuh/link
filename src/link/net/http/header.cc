/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/net/http/header.h"

#include <sstream>

namespace nlink {
namespace net {
namespace http {

Header::Header() {
}

Header::~Header() {
}

void Header::Set(const std::pair<std::string, std::string>& header) {
  header_fields_.insert(header);
}

void Header::Set(const std::string& key, const std::string& value) {
  header_fields_.insert({key, value});
}

const std::string Header::Serialize() const {
  std::stringstream stream;
  for (const auto& header : header_fields_) {
    const std::string& key = header.first;
    const std::string& value = header.second;

    if (key.empty() || value.empty()) {
      continue;
    }

    stream << key;
    stream << ":";
    stream << value;
    stream << "\r\n";
  }
  return stream.str();
}

bool Header::Empty() const {
  return header_fields_.empty();
}

const std::string Header::Find(const std::string& key) const {
  if (header_fields_.find(key) == header_fields_.end()) {
    return "";
  }
  return header_fields_.at(key);
}


}  // namespace http
}  // namespace net
}  // namespace nlink

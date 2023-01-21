/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>

#include <link/base/base64.h>

using namespace nlink;

TEST(Base64, encode) {
  std::string source("");
  std::string encoded("");
  std::string target("");

  source = "test message";
  target = "dGVzdCBtZXNzYWdl";
  encoded = base::base64::Encode(source);
  EXPECT_STRCASEEQ(target.c_str(), encoded.c_str());

  source = "{\"Accept-Language\": \"en-US,en;q=0.8\"}";
  target = "eyJBY2NlcHQtTGFuZ3VhZ2UiOiAiZW4tVVMsZW47cT0wLjgifQ==";
  encoded = base::base64::Encode(source);
  EXPECT_STRCASEEQ(target.c_str(), encoded.c_str());
}

TEST(Base64, decode) {
  std::string source("");
  std::string decoded("");
  std::string target("");

  source = "dGVzdCBtZXNzYWdl";
  target = "test message";
  decoded = base::base64::Decode(source);
  EXPECT_STRCASEEQ(target.c_str(), decoded.c_str());

  source = "eyJBY2NlcHQtTGFuZ3VhZ2UiOiAiZW4tVVMsZW47cT0wLjgifQ==";
  target = "{\"Accept-Language\": \"en-US,en;q=0.8\"}";
  decoded = base::base64::Decode(source);
  EXPECT_STRCASEEQ(target.c_str(), decoded.c_str());
}

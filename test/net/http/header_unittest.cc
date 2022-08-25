/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include <string>
#include <sstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <link/net/http/header.h>

using namespace nlink;

TEST(HttpHeader, set_header) {
  // basic set header
  {
    net::http::HttpHeader header;

    header.Set("content-type", "text/html");
    EXPECT_STRCASEEQ("text/html", header.Find("content-type").c_str());
    EXPECT_STRCASEEQ("text/html", header.Find("Content-type").c_str());

    header.Set("content-type", "application/json");
    EXPECT_STRCASEEQ("application/json", header.Find("content-type").c_str());

    header.Set("content-length", 111);
    EXPECT_STRCASEEQ("111", header.Find("content-length").c_str());
  }

  // header string parse and set
  {
    std::string content_encoding = "content-encoding: br";
    std::string content_length = "content-length: 620";
    std::string content_type =
      "content-type: application/javascript; charset=utf-8";

    net::http::HttpHeader header;
    EXPECT_TRUE(header.Set(content_encoding));
    EXPECT_TRUE(header.Set(content_length));
    EXPECT_TRUE(header.Set(content_type));

    EXPECT_STRCASEEQ("br", header.Find("content-encoding").c_str());
    EXPECT_STRCASEEQ("620", header.Find("content-length").c_str());
    EXPECT_STRCASEEQ(
      "application/javascript; charset=utf-8",
      header.Find("content-type").c_str());
  }
}

TEST(HttpHeader, fail_set_header) {
  // header string parse and set
  {
    std::string content_encoding = "";
    std::string content_length = "content-length:";
    std::string content_type =
      ":application/javascript";
    std::string accept = "accept:*/*";

    net::http::HttpHeader header;
    EXPECT_FALSE(header.Set(content_encoding));
    EXPECT_FALSE(header.Set(content_length));
    EXPECT_FALSE(header.Set(content_type));
    EXPECT_FALSE(header.Set(accept));
  }
}

TEST(HttpHeader, serialize_header) {
  // header serialize
  {
    std::string CRLF = "\r\n";
    std::stringstream stream;

    std::string content_type =
      "content-type: application/javascript; charset=utf-8";
    std::string content_length = "content-length: 620";
    std::string default_user_agent = "user-agent: nLink 0.0.1";
    std::string content_encoding = "content-encoding: br";

    stream << content_type << CRLF;
    stream << content_length << CRLF;
    stream << default_user_agent << CRLF;
    stream << content_encoding << CRLF;

    net::http::HttpHeader header;
    EXPECT_TRUE(header.Set(content_encoding));
    EXPECT_TRUE(header.Set(content_length));
    EXPECT_TRUE(header.Set(content_type));

    EXPECT_STRCASEEQ(stream.str().c_str(), header.Serialize().c_str());
  }
}

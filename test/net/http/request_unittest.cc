/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include <string>
#include <sstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <link/net/http/request.h>

using namespace nlink;

const char* kContentTypeKey = "content-type";
const char* kContentLengthKey = "content-length";

TEST(HttpReqeust, default_contruct_request) {
  // empty request
  {
    net::http::Request request;
    EXPECT_FALSE(request.IsValid());
  }

  // invliad request line 1
  {
    net::Uri uri = net::Uri::Parse("https://www.google.com");
    net::http::Request::RequestLine request_line =
      {net::http::Method::INVALID, uri, net::http::Version::HTTP_1_1};
    net::http::Request request(request_line);
    EXPECT_FALSE(request.IsValid());
  }

  // invliad request line 1
  {
    net::http::Request::RequestLine request_line =
      {net::http::Method::GET, net::Uri(), net::http::Version::INVALID};
    net::http::Request request(request_line);
    EXPECT_FALSE(request.IsValid());
  }

  // invliad request line 1
  {
    net::Uri uri = net::Uri::Parse("https://www.google.com");
    net::http::Request::RequestLine request_line =
      {net::http::Method::GET, uri, net::http::Version::INVALID};
    net::http::Request request(request_line);
    EXPECT_FALSE(request.IsValid());
  }

  // valid request line
  {
    net::Uri uri = net::Uri::Parse("https://www.google.com");
    net::http::Request::RequestLine request_line =
      {net::http::Method::GET, uri, net::http::Version::HTTP_1_1};
    net::http::Request request(request_line);
    EXPECT_TRUE(request.IsValid());
  }
}

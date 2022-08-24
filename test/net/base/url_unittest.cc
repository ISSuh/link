/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include <string>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <link/net/base/uri.h>

using namespace nlink;

TEST(Url, parse_uri_string_succes) {
  std::string uri_str;
  net::Uri uri;

  uri_str = "http://www.google.com";
  uri = net::Uri::Parse(uri_str);
  EXPECT_TRUE(uri.HasScheme());
  EXPECT_TRUE(uri.HasHost());
  EXPECT_STRCASEEQ("http", uri.Scheme().c_str());
  EXPECT_STRCASEEQ("www.google.com", uri.Host().c_str());
  EXPECT_STRCASEEQ(uri_str.c_str(), uri.Serialize().c_str());

  uri_str = "https://localhost:12345";
  uri = net::Uri::Parse(uri_str);
  EXPECT_TRUE(uri.HasScheme());
  EXPECT_TRUE(uri.HasHost());
  EXPECT_TRUE(uri.HasPort());
  EXPECT_STRCASEEQ("https", uri.Scheme().c_str());
  EXPECT_STRCASEEQ("localhost", uri.Host().c_str());
  EXPECT_EQ(12345, uri.Port());
  EXPECT_STRCASEEQ(uri_str.c_str(), uri.Serialize().c_str());

  uri_str = "https://user@localhost";
  uri = net::Uri::Parse(uri_str);
  EXPECT_TRUE(uri.HasScheme());
  EXPECT_TRUE(uri.HasUserName());
  EXPECT_TRUE(uri.HasHost());
  EXPECT_STRCASEEQ("https", uri.Scheme().c_str());
  EXPECT_STRCASEEQ("user", uri.UserName().c_str());
  EXPECT_STRCASEEQ("localhost", uri.Host().c_str());
  EXPECT_STRCASEEQ(uri_str.c_str(), uri.Serialize().c_str());

  uri_str = "http://user:password@localhost:12345";
  uri = net::Uri::Parse(uri_str);
  EXPECT_TRUE(uri.HasScheme());
  EXPECT_TRUE(uri.HasUserName());
  EXPECT_TRUE(uri.HasPassword());
  EXPECT_TRUE(uri.HasHost());
  EXPECT_TRUE(uri.HasPort());
  EXPECT_STRCASEEQ("http", uri.Scheme().c_str());
  EXPECT_STRCASEEQ("user", uri.UserName().c_str());
  EXPECT_STRCASEEQ("password", uri.Password().c_str());
  EXPECT_STRCASEEQ("localhost", uri.Host().c_str());
  EXPECT_EQ(12345, uri.Port());
  EXPECT_STRCASEEQ(uri_str.c_str(), uri.Serialize().c_str());

  uri_str = "http://localhost:80/test";
  uri = net::Uri::Parse(uri_str);
  EXPECT_TRUE(uri.HasScheme());
  EXPECT_TRUE(uri.HasHost());
  EXPECT_TRUE(uri.HasPort());
  EXPECT_TRUE(uri.HasPath());
  EXPECT_STRCASEEQ("http", uri.Scheme().c_str());
  EXPECT_STRCASEEQ("localhost", uri.Host().c_str());
  EXPECT_EQ(80, uri.Port());
  EXPECT_STRCASEEQ("/test", uri.Path().c_str());
  EXPECT_STRCASEEQ(uri_str.c_str(), uri.Serialize().c_str());

  uri_str = "http://localhost:80/test1/test2?value1=1&value2=2";
  uri = net::Uri::Parse(uri_str);
  EXPECT_TRUE(uri.HasScheme());
  EXPECT_TRUE(uri.HasHost());
  EXPECT_TRUE(uri.HasPort());
  EXPECT_TRUE(uri.HasPath());
  EXPECT_TRUE(uri.HasQuery());
  EXPECT_STRCASEEQ("http", uri.Scheme().c_str());
  EXPECT_STRCASEEQ("localhost", uri.Host().c_str());
  EXPECT_EQ(80, uri.Port());
  EXPECT_STRCASEEQ("/test1/test2", uri.Path().c_str());
  EXPECT_STRCASEEQ("value1=1&value2=2", uri.QueryString().c_str());
  EXPECT_STRCASEEQ(
    "/test1/test2?value1=1&value2=2", uri.PathWithQueryAndFragment().c_str());
  EXPECT_STRCASEEQ(uri_str.c_str(), uri.Serialize().c_str());

  uri_str = "http://localhost:80/test1?value1=1#title1";
  uri = net::Uri::Parse(uri_str);
  EXPECT_TRUE(uri.HasScheme());
  EXPECT_TRUE(uri.HasHost());
  EXPECT_TRUE(uri.HasPort());
  EXPECT_TRUE(uri.HasPath());
  EXPECT_TRUE(uri.HasQuery());
  EXPECT_TRUE(uri.HasFragment());
  EXPECT_STRCASEEQ("http", uri.Scheme().c_str());
  EXPECT_STRCASEEQ("localhost", uri.Host().c_str());
  EXPECT_EQ(80, uri.Port());
  EXPECT_STRCASEEQ("/test1", uri.Path().c_str());
  EXPECT_STRCASEEQ("value1=1", uri.QueryString().c_str());
  EXPECT_STRCASEEQ("title1", uri.Fragment().c_str());
  EXPECT_STRCASEEQ(
    "/test1?value1=1#title1", uri.PathWithQueryAndFragment().c_str());
  EXPECT_STRCASEEQ(uri_str.c_str(), uri.Serialize().c_str());
}

TEST(Url, parse_uri_string_fail) {
  std::string uri_str;
  net::Uri uri;

  // without scheme
  uri_str = "www.google.com";
  uri = net::Uri::Parse(uri_str);
  EXPECT_TRUE(uri.IsEmpty());

  // invalid scheme delimeter
  uri_str = "http:/www.google.com";
  uri = net::Uri::Parse(uri_str);
  EXPECT_TRUE(uri.IsEmpty());

  // without host
  uri_str = "http://";
  uri = net::Uri::Parse(uri_str);
  EXPECT_TRUE(uri.IsEmpty());

  // invalid port
  uri_str = "http://localhost:80tt";
  uri = net::Uri::Parse(uri_str);
  EXPECT_TRUE(uri.IsEmpty());
}

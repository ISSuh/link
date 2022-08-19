/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include <string>
#include <vector>
#include <iostream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <link/net/http/handler.h>
#include <link/net/http/routeing.h>

using namespace nlink;

TEST(Rounting, Parse_path) {
  std::vector<net::http::Routing::SplitedPath> paths;

  std::string empty_path("");
  net::http::SplitPathBySlash(empty_path, &paths);
  EXPECT_TRUE(paths.empty());

  std::string root_path("/");
  net::http::SplitPathBySlash(root_path, &paths);
  EXPECT_TRUE(paths.empty());

  std::string path_1("/foo/bar");
  std::vector<net::http::Routing::SplitedPath> temp_paths_1 =
    {{"foo", false}, {"bar", false}};
  net::http::SplitPathBySlash(path_1, &paths);
  for (size_t i = 0 ; i < paths.size() ; ++i) {
    EXPECT_STREQ(paths[i].first.c_str(), temp_paths_1[i].first.c_str());
    EXPECT_EQ(paths[i].second, temp_paths_1[i].second);
  }
  paths.clear();

  std::string path_2("/foo/bar/<id>");
  std::vector<net::http::Routing::SplitedPath> temp_paths_2 =
    {{"foo", false}, {"bar", false}, {"<id>", true}};
  net::http::SplitPathBySlash(path_2, &paths);
  for (size_t i = 0 ; i < paths.size() ; ++i) {
    EXPECT_STREQ(paths[i].first.c_str(), temp_paths_2[i].first.c_str());
    EXPECT_EQ(paths[i].second, temp_paths_2[i].second);
  }
}

class RoutingHandler {
 public:
  virtual void Root(const net::http::Request&, net::http::Response*) = 0;
  virtual void Foo(const net::http::Request&, net::http::Response*) = 0;
  virtual void FooParam(const net::http::Request&, net::http::Response*) = 0;
  virtual void FooBarBaz(const net::http::Request&, net::http::Response*) = 0;
  virtual void FooParamBaz(const net::http::Request&, net::http::Response*) = 0;
};

class RoutingHandlerMock : public RoutingHandler {
 public:
  MOCK_METHOD2(
    Root, void(const net::http::Request&, net::http::Response*));
  MOCK_METHOD2(
    Foo, void(const net::http::Request&, net::http::Response*));
  MOCK_METHOD2(
    FooParam, void(const net::http::Request&, net::http::Response*));
  MOCK_METHOD2(
    FooBarBaz, void(const net::http::Request&, net::http::Response*));
  MOCK_METHOD2(
    FooParamBaz, void(const net::http::Request&, net::http::Response*));
};

TEST(Rounting, regist_handler) {
  net::http::Request dummy_request;
  net::http::Response dummy_response;

  RoutingHandlerMock mock_handler;
  net::http::Routing routing;
  net::http::handler::ResponseHandler routing_handler = {};

  // test path /
  std::string root_path("/");
  routing_handler = routing.Route(root_path);
  EXPECT_FALSE(routing_handler);

  routing.RegistHandler(
    root_path,
    std::bind(&RoutingHandlerMock::Root, &mock_handler,
      std::placeholders::_1, std::placeholders::_2));

  routing_handler = routing.Route(root_path);
  EXPECT_TRUE(routing_handler);
  EXPECT_CALL(mock_handler, Root(testing::_, testing::_)).Times(1);
  routing_handler(dummy_request, &dummy_response);

  // test path /foo
  std::string foo_path("/foo");
  routing.RegistHandler(
    foo_path,
    std::bind(&RoutingHandlerMock::Foo, &mock_handler,
      std::placeholders::_1, std::placeholders::_2));

  routing_handler = routing.Route(foo_path);
  EXPECT_TRUE(routing_handler);
  EXPECT_CALL(mock_handler, Foo(testing::_, testing::_)).Times(1);
  routing_handler(dummy_request, &dummy_response);

  // test path /foo/<test_id>
  std::string foo_param_path("/foo/<test_id>");
  routing.RegistHandler(
    foo_param_path,
    std::bind(&RoutingHandlerMock::FooParam, &mock_handler,
      std::placeholders::_1, std::placeholders::_2));

  std::string foo_my_id_path("/foo/my_id");
  routing_handler = routing.Route(foo_my_id_path);
  EXPECT_TRUE(routing_handler);
  EXPECT_CALL(mock_handler, FooParam(testing::_, testing::_)).Times(1);
  routing_handler(dummy_request, &dummy_response);

  std::string foo_your_id_path("/foo/your_id");
  routing_handler = routing.Route(foo_your_id_path);
  EXPECT_TRUE(routing_handler);
  EXPECT_CALL(mock_handler, FooParam(testing::_, testing::_)).Times(1);
  routing_handler(dummy_request, &dummy_response);

  // test path /foo/bar/baz
  std::string foo_bar_baz_path("/foo/bar/baz");
  routing.RegistHandler(
    foo_bar_baz_path,
    std::bind(&RoutingHandlerMock::FooBarBaz, &mock_handler,
      std::placeholders::_1, std::placeholders::_2));

  routing_handler = routing.Route(foo_bar_baz_path);
  EXPECT_TRUE(routing_handler);
  EXPECT_CALL(mock_handler, FooBarBaz(testing::_, testing::_)).Times(1);
  routing_handler(dummy_request, &dummy_response);

  // test path /foo/<test_id>/baz
  std::string foo_id_baz_path("/foo/my_id/baz");
  routing_handler = routing.Route(foo_id_baz_path);
  EXPECT_FALSE(routing_handler);

  routing.RegistHandler(
    foo_id_baz_path,
    std::bind(&RoutingHandlerMock::FooParamBaz, &mock_handler,
      std::placeholders::_1, std::placeholders::_2));

  routing_handler = routing.Route(foo_id_baz_path);
  EXPECT_TRUE(routing_handler);
  EXPECT_CALL(
    mock_handler,
    FooParamBaz(testing::_, testing::_)).Times(1);
  routing_handler(dummy_request, &dummy_response);
}

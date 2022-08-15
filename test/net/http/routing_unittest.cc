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

#include <link/net/http/routeing.h>

TEST(Parse_path, Rounting) {
  std::vector<nlink::net::http::Routing::SplitedPath> paths;

  std::string empty_path("");
  nlink::net::http::SplitPathBySlash(empty_path, &paths);
  EXPECT_TRUE(paths.empty());

  std::string root_path("/");
  nlink::net::http::SplitPathBySlash(root_path, &paths);
  EXPECT_TRUE(paths.empty());

  std::string path_1("/foo/bar");
  std::vector<nlink::net::http::Routing::SplitedPath> temp_paths_1 =
    {{"foo", false}, {"bar", false}};
  nlink::net::http::SplitPathBySlash(path_1, &paths);
  for (size_t i = 0 ; i < paths.size() ; ++i) {
    EXPECT_STREQ(paths[i].first.c_str(), temp_paths_1[i].first.c_str());
    EXPECT_EQ(paths[i].second, temp_paths_1[i].second);
  }
  paths.clear();

  std::string path_2("/foo/bar/<id>");
  std::vector<nlink::net::http::Routing::SplitedPath> temp_paths_2 =
    {{"foo", false}, {"bar", false}, {"<id>", true}};
  nlink::net::http::SplitPathBySlash(path_2, &paths);
  for (size_t i = 0 ; i < paths.size() ; ++i) {
    EXPECT_STREQ(paths[i].first.c_str(), temp_paths_2[i].first.c_str());
    EXPECT_EQ(paths[i].second, temp_paths_2[i].second);
  }
}

class RoutingHandler {
 public:
  virtual void Root() = 0;
  virtual void Foo() = 0;
  virtual void FooParam() = 0;
  virtual void FooBarBaz() = 0;
};

class RoutingHandlerMock : public RoutingHandler {
 public:
  MOCK_METHOD0(Root, void());
  MOCK_METHOD0(Foo, void());
  MOCK_METHOD0(FooParam, void());
  MOCK_METHOD0(FooBarBaz, void());
};

TEST(regist_handler, Rounting) {
  RoutingHandlerMock mock_handler;
  nlink::net::http::Routing routing;
  std::function<void()> routing_handler = {};

  // test path /
  std::string root_path("/");
  routing_handler = routing.Route(root_path);
  EXPECT_FALSE(routing_handler);

  routing.RegistHandler(
    root_path, std::bind(&RoutingHandlerMock::Root, &mock_handler));

  routing_handler = routing.Route(root_path);
  EXPECT_TRUE(routing_handler);
  EXPECT_CALL(mock_handler, Root()).Times(1);
  routing_handler();

  // test path /foo
  std::string foo_path("/foo");
  routing.RegistHandler(
    foo_path, std::bind(&RoutingHandlerMock::Foo, &mock_handler));

  routing_handler = routing.Route(foo_path);
  EXPECT_TRUE(routing_handler);
  EXPECT_CALL(mock_handler, Foo()).Times(1);
  routing_handler();

  // test path /foo/<test_id>
  std::string foo_param_path("/foo/<test_id>");
  routing.RegistHandler(
    foo_param_path,
    std::bind(&RoutingHandlerMock::FooParam, &mock_handler));

  std::string foo_id_path("/foo/my_id");
  routing_handler = routing.Route(foo_id_path);
  EXPECT_TRUE(routing_handler);
  EXPECT_CALL(mock_handler, FooParam()).Times(1);
  routing_handler();

  // test path /foo/bar/baz
  std::string foo_bar_baz_path("/foo/bar/baz");
  routing.RegistHandler(
    foo_bar_baz_path, std::bind(&RoutingHandlerMock::FooBarBaz, &mock_handler));

  routing_handler = routing.Route(foo_bar_baz_path);
  EXPECT_TRUE(routing_handler);
  EXPECT_CALL(mock_handler, FooBarBaz()).Times(1);
  routing_handler();

  std::string foo_id_baz_path("/foo/my_id/baz");
  routing_handler = routing.Route(foo_id_baz_path);
  EXPECT_TRUE(routing_handler);
}

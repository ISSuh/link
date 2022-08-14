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
  std::vector<std::string> paths;

  std::string empty_path("");
  nlink::net::http::SplitPathBySlash(empty_path, &paths);
  EXPECT_TRUE(paths.empty());

  std::string root_path("/");
  nlink::net::http::SplitPathBySlash(root_path, &paths);
  EXPECT_TRUE(paths.empty());

  std::string path_1("/foo/bar");
  std::vector<std::string> temp_paths_1 = {"foo", "bar"};
  nlink::net::http::SplitPathBySlash(path_1, &paths);
  for (size_t i = 0 ; i < paths.size() ; ++i) {
    EXPECT_STREQ(paths[i].c_str(), temp_paths_1[i].c_str());
  }
  paths.clear();

  std::string path_2("/foo/bar/{:id}");
  std::vector<std::string> temp_paths_2 = {"foo", "bar", "{:id}"};
  nlink::net::http::SplitPathBySlash(path_2, &paths);
  for (size_t i = 0 ; i < paths.size() ; ++i) {
    EXPECT_STREQ(paths[i].c_str(), temp_paths_2[i].c_str());
  }
}

class RoutingHandler {
 public:
  virtual void Root() = 0;
  virtual void Foo() = 0;
  virtual void FooBar() = 0;
};

class RoutingHandlerMock : public RoutingHandler {
 public:
  MOCK_METHOD0(Root, void());
  MOCK_METHOD0(Foo, void());
  MOCK_METHOD0(FooBar, void());
};

TEST(regist_handler, Rounting) {
  RoutingHandlerMock mock_handler;
  nlink::net::http::Routing routing;

  std::string root_path("/");
  auto routing_handler_0 = routing.Route(root_path);
  EXPECT_FALSE(routing_handler_0);

  routing.RegistHandler(
    root_path, std::bind(&RoutingHandlerMock::Root, &mock_handler));

  auto routing_handler_1 = routing.Route(root_path);
  EXPECT_TRUE(routing_handler_1);
  EXPECT_CALL(mock_handler, Root()).Times(1);
  routing_handler_1();

  std::string foo_path("/foo");
  routing.RegistHandler(
    foo_path, std::bind(&RoutingHandlerMock::Foo, &mock_handler));

  auto routing_handler_2 = routing.Route(foo_path);
  EXPECT_TRUE(routing_handler_2);
  EXPECT_CALL(mock_handler, Foo()).Times(1);
  routing_handler_2();

  std::string foo_bar_path("/foo/bar");
  routing.RegistHandler(
    foo_bar_path, std::bind(&RoutingHandlerMock::FooBar, &mock_handler));

  auto routing_handler_3 = routing.Route(foo_bar_path);
  EXPECT_TRUE(routing_handler_3);
  EXPECT_CALL(mock_handler, FooBar()).Times(1);
  routing_handler_3();

  std::string foo_test_path("/foo/test");
  auto routing_handler_4 = routing.Route(foo_test_path);
  EXPECT_FALSE(routing_handler_4);

  std::string foo_bar_test_path("/foo/bar/test");
  auto routing_handler_5 = routing.Route(foo_bar_test_path);
  EXPECT_FALSE(routing_handler_5);
}

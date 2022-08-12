/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include <string>

#include <gtest/gtest.h>

#include <link/io/base/ip_address.h>

TEST(Empty, IpAddress) {
  nlink::io::IpAddress ip_addr;

  EXPECT_FALSE(ip_addr.IsIPv4());
  EXPECT_FALSE(ip_addr.IsIPv6());
  EXPECT_FALSE(ip_addr.IsZero());
  EXPECT_FALSE(ip_addr.IsLoopback());
  EXPECT_FALSE(ip_addr.IsDomainName());

  EXPECT_EQ(0, ip_addr.Size());
  EXPECT_TRUE(ip_addr.Empty());
}

TEST(Parse_ipv4_loopback, IpAddress) {
  nlink::io::IpAddress loop_back = nlink::io::ParseAddress("127.0.0.1");
  EXPECT_FALSE(loop_back.Empty());
  EXPECT_TRUE(loop_back.IsLoopback());
  EXPECT_TRUE(loop_back.IsIPv4());

  std::string origin = loop_back.Origin();
  EXPECT_TRUE(origin.compare("127.0.0.1") != std::string::npos);
}

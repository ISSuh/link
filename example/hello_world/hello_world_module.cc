/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "hello_world_module.h"

#include <link/base/logging.h>

#include <link/net/http/request.h>
#include <link/net/http/request_parser.h>
#include <link/net/http/parser.h>

using namespace nlink;

#define REQ                                                                                                                        \
    "GET /wp-content/uploads/2010/03/hello-kitty-darth-vader-pink.jpg HTTP/1.1\r\n"                                                \
    "Host: www.kittyhell.com\r\n"                                                                                                  \
    "User-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.6; ja-JP-mac; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3 "             \
    "Pathtraq/0.9\r\n"                                                                                                             \
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"                                                  \
    "Accept-Language: ja,en-us;q=0.7,en;q=0.3\r\n"                                                                                 \
    "Accept-Encoding: gzip,deflate\r\n"                                                                                            \
    "Accept-Charset: Shift_JIS,utf-8;q=0.7,*;q=0.7\r\n"                                                                            \
    "Keep-Alive: 115\r\n"                                                                                                          \
    "Connection: keep-alive\r\n"                                                                                                   \
    "Cookie: wp_ozh_wsa_visits=2; wp_ozh_wsa_visit_lasttime=xxxxxxxxxx; "                                                          \
    "__utma=xxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.x; "                                                             \
    "__utmz=xxxxxxxxx.xxxxxxxxxx.x.x.utmccn=(referral)|utmcsr=reader.livedoor.com|utmcct=/reader/|utmcmd=referral\r\n"             \
    "\r\n"      \
    "hello world"


void SampleModule::Init() {
  LOG(INFO) << "SampleModule::init";

  GetArgument("int_test", &int_test_);
  GetArgument("float_test", &float_test_);
  GetArgument("bool_test", &bool_test_);
  GetArgument("string_test", &string_test_);
}

void SampleModule::Run() {
  LOG(INFO) << "SampleModule::Run";

  LOG(INFO) << "int_test_ : " << int_test_;
  LOG(INFO) << "float_test_ : " << float_test_;
  LOG(INFO) << "bool_test_ : " << bool_test_;
  LOG(INFO) << "string_test_ : " << string_test_;

  net::Uri uri;
  const std::string http_uri =
    // "https://user:password@www.google.com:443/webhp/poo?gws_rd=ssl&a=b&s=c#test";
    "https://myname:1234@www.google.com:443/webhp/poo?gws_rd=ssl&a=b&s=c#test";

  uri.Decode(http_uri);
  uri.PrintForDebug();

  LOG(INFO) << uri.Encode();

  net::http::Header header;
  header.Set({"Accept", "text/html"});
  header.Set({"Accept-Language", "en-us"});
  header.Set(
    {"Cookie", "wp_ozh_wsa_visits=2; wp_ozh_wsa_visit_lasttime=xxxxxxxxxx;"});

  net::http::Request request(
    net::http::Method::GET, uri, net::http::Version::HTTP_1_1,
    header, "Hello World");
  // request.SetBody();
  LOG(INFO) << "\n" << request.Serialize();

  base::Buffer buffer(REQ);
  net::http::Request parsed_request = net::http::RequestParser::Parse(buffer);

  LOG(INFO) << "\n" << parsed_request.Serialize();
}

void SampleModule::Shutdown() {
  LOG(INFO) << "SampleModule::Terminate";
}

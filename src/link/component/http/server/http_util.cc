
/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/component/http/server/http_util.h"

#include <string>
#include <sstream>

#include "link/net/http/status_code.h"

namespace nlink {
namespace component {
namespace http {

net::http::Response Default404Error() {
  std::stringstream body_stream;
  body_stream << "<html>" << '\n';
  body_stream << "  <head>" << '\n';
  body_stream << "  </head>" << '\n';
  body_stream << "  <body>" << '\n';
  body_stream << "    <p>404 not found</p>" << '\n';
  body_stream << "  </body>" << '\n';
  body_stream << "</html>" << '\n';

  net::http::HttpHeader header;
  header.Set("Content-Type", "text/html");
  header.Set("Content-Length", body_stream.str().size());

  return net::http::Response(
    net::http::HttpStatusCode::NOT_FOUND,
    net::http::Version::HTTP_1_1,
    header,
    body_stream.str());
}

}  // namespace http
}  // namespace component
}  // namespace nlink

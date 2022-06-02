/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_LOGGING_H_
#define LINK_BASE_LOGGING_H_

#include <string>

#include "link/third_party/glog/logging.h"

namespace nlink {
namespace base {

class LinkLogger {
 public:
  LinkLogger() {
    google::InitGoogleLogging("--logtostderr=1");
  }

  ~LinkLogger() {
    google::ShutdownGoogleLogging();
  }
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_LOGGING_H_

/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include <signal.h>

#include "link/node/base/node.h"
#include "link/node/base/arguments.h"
#include "link/base/logging.h"
#include "link/base/stack_trace.h"

using namespace nlink;

void handler(int32_t) {
  base::PrintStackTrace();
  exit(0);
}

int main(int32_t argc, char *argv[]) {
  signal(SIGSEGV, handler);   // install our handler

  node::LinkNode node;
  if (!node.Init(argc, argv)) {
    exit(0);
  }

  node.Run();
}

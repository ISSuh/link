/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/node/base/node.h"
#include "link/node/base/arguments.h"
#include "link/base/logging.h"

using namespace nlink;

int main(int32_t argc, char *argv[]) {
  node::LinkNode node;
  if (!node.Init(argc, argv)) {
    exit(0);
  }

  node.Run();
}

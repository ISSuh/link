/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/node/base/node.h"

namespace link {
namespace node {

LinkNode::LinkNode()
  : controller_(nullptr),
    args_(nullptr) {
}

LinkNode::~LinkNode() {
}

bool LinkNode::Init(int32_t argc, char *argv[]) {
  Arguments args(argc, argv);
}

void LinkNode::Run() {
}

}  // namespace node
}  // namespace link

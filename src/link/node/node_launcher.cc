/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/node/base/node.h"
#include "link/node/base/arguments.h"
#include "link/base/logging.h"

using namespace link;

void main(int32_t argc, char *argv[]) {
  LOG(INFO) << "Node Launcher";

  node::LinkNode node;
  if (node.Init(argc, argv)) {
    exit(0);
  }

  return node.Run();
  // const std::string TEST_MODULE_JSON_PATH  = args.module_config_path();

  // std::ifstream json_file(TEST_MODULE_JSON_PATH);
  // std::string json_str((std::istream_iterator<char>(json_file)),
  //                     std::istream_iterator<char>());

  // base::JsonWrapper spec_json(json_str);

  // module::Specification spec;
  // spec.ParseFromStr(spec_json["modules"][0].dump());

  // module::ModuleLoader loader;
  // loader.LoadModule(spec);

  // module::Module* user_module = loader.GetModule("Sample_0");
  // // user_module->RunModule();

  // loader.UnLoadAllModule();
}

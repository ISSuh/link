/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include <iostream>
#include <fstream>
#include <streambuf>

#include "link/module/loader/module_loader.h"
#include "link/base/logging.h"
#include "link/base/json_wrapper.h"

using namespace link;

int32_t main(int32_t argc, char *argv[]) {
  std::cout << "Link Example" << std::endl;

  const std::string TEST_MODULE_JSON_PATH =
    "/home/issuh/workspace/my_project/link/example/hello_world/hello_world_conf.json";

  std::ifstream json_file(TEST_MODULE_JSON_PATH);
  std::string json_str((std::istream_iterator<char>(json_file)),
                      std::istream_iterator<char>());

  base::JsonWrapper spec_json(json_str);

  module::Module::Specification spec;
  spec.ParseFromStr(spec_json["modules"][0].dump());

  module::ModuleLoader loader;
  loader.LoadModule(spec);

  module::Module* user_module = loader.GetModule("Sample_0");
  user_module->RunModule();

  loader.UnLoadAllModule();
}

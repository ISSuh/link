/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include <iostream>
#include <fstream>
#include <streambuf>

#include "link/module/loader/module_loader_manager.h"

using namespace link;

int32_t main(int32_t argc, char *argv[]) {
  std::cout << "SimpleModule Node" << std::endl;

  const std::string TEST_MODULE_JSON_PATH =
      "/home/issuh/workspace/myProject/SimpleModule/example/hello_world/hello_world_conf.json";

  std::ifstream json_file(TEST_MODULE_JSON_PATH);
  std::string json_str((std::istream_iterator<char>(json_file)),
                      std::istream_iterator<char>());

  launcher::ModuleLoadManager loader_manager;
  loaderManager.createModlueLoader(base::JsonWrapper(json_str));

  loaderManager.loadAllModule();

  auto user_module = loaderManager.getModule("Sample_0");

  user_module->initialize();
  user_module->process();
  user_module->shutdown();

  loaderManager.unLoadAllModule();
}

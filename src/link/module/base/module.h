/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_MODULE_BASE_MODULE_H_
#define LINK_MODULE_BASE_MODULE_H_

#include <string>
#include <memory>

#include "link/base/macro.h"
#include "link/base/json_wrapper.h"
#include "link/base/task/task_runner.h"
#include "link/module/base/module_client.h"
#include "link/module/base/specification.h"

namespace nlink {
namespace module {

class LinkModule;

using LinkModulePtr =
  std::unique_ptr<LinkModule, std::function<void(LinkModule*)>>;

class LinkModule {
 public:
  virtual ~LinkModule() = default;

  static LinkModulePtr CreateModule(
    base::TaskRunner* task_runner,
    ModuleClient* client,
    const Specification& spec);
  const Specification ModuleSpecification() const;

  virtual void Initialize() = 0;
  virtual void Process() = 0;
  virtual void Terminate() = 0;
  virtual bool IsRunning() const = 0;

 protected:
  explicit LinkModule(const Specification& spec);

  const Specification spec_;

  DISAALOW_COPY_AND_ASSIGN(LinkModule)
};

}  // namespace module
}  // namespace nlink

#endif  // LINK_MODULE_BASE_MODULE_H_

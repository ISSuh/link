/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_TASK_TASK_DISPATCHER_H_
#define LINK_BASE_TASK_TASK_DISPATCHER_H_

#include <string>
#include <vector>
#include <map>

#include "link/base/task_manager.h"
#include "link/base/callback.h"
#include "link/base/time.h"

namespace nlink {
namespace base {

class TaskManager;

class TaskDispatcher {
 public:
  explicit TaskDispatcher(TaskManager* manager);
  ~TaskDispatcher();

  void PostTask(
    const std::string& group,
    const std::string& label,
    TaskCallback task);

  void PostDelayTask(
    const std::string& group,
    const std::string& label,
    TaskCallback task,
    TimeTick delay);

 private:
  TaskManager* manager_;
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_TASK_TASK_DISPATCHER_H_

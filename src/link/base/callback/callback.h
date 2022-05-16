/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_CALLBACK_CALLBACK_H_
#define LINK_BASE_CALLBACK_CALLBACK_H_

#include <utility>

#include "link/base/callback/bind.h"

namespace nlink {
namespace base {

class BindStateBase;

class CallbackBase {
 public:
  CallbackBase(const CallbackBase& c)
    : bind_state_(c.bind_state_) {}

  CallbackBase(CallbackBase&& c) noexcept
    : bind_state_(c.bind_state_) {}

  bool is_null() const { return !bind_state_; }
  explicit operator bool() const { return !is_null(); }

  void Reset() {
    bind_state_ = nullptr;
  }

  CallbackBase& operator=(const CallbackBase& c) {
    bind_state_ = c.bind_state_;
    return *this;
  }

  CallbackBase& operator=(CallbackBase&& c) noexcept = default;

 protected:
  using InvokeFuncStorage = BindStateBase::InvokeFuncStorage;

  bool EqualsInternal(const CallbackBase& other) const {
    return bind_state_ == other.bind_state_;
  }

  CallbackBase() : bind_state_(nullptr) {}

  explicit inline CallbackBase(BindStateBase* bind_state)
    : bind_state_(bind_state) {}

  ~CallbackBase() = default;

  InvokeFuncStorage polymorphic_invoke() const {
    return bind_state_->polymorphic_invoke_;
  }


  BindStateBase* bind_state_;
};

template <typename R, typename... Args>
class Callback<R(Args...)> : public CallbackBase {
 public:
  using RunType = R(Args...);
  using PolymorphicInvoke = R(*)(BindStateBase*, PassingType<Args>...);

  constexpr Callback() = default;
  Callback(std::nullptr_t) = delete;

  explicit Callback(BindStateBase* bind_state)
      : CallbackBase(bind_state) {}

  // explicit Callback(const Callback<RunType>& other)
  //     : CallbackBase(std::move(other)) {}

  Callback(const Callback&) = default;
  Callback& operator=(const Callback&) = default;

  Callback(Callback&&) noexcept = default;
  Callback& operator=(Callback&&) noexcept = default;

  R Run(Args... args) const & {
    PolymorphicInvoke f =
        reinterpret_cast<PolymorphicInvoke>(this->polymorphic_invoke());
    return f(this->bind_state_, std::forward<Args>(args)...);
  }
};

using TaskCallback = Callback<void()>;
using CompletionCallback = Callback<void(int32_t)>;

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_CALLBACK_CALLBACK_H_

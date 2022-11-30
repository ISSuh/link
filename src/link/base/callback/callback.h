/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_CALLBACK_TEST_CALLBACK_H_
#define LINK_BASE_CALLBACK_TEST_CALLBACK_H_

#include <iostream>

#include <memory>
#include <utility>
#include <type_traits>
#include <typeinfo>

std::string demangle(const char* name);

namespace nlink {
namespace base {
namespace detail {

template <typename R, typename... Args>
class CallbackImplBase {
 public:
  virtual ~CallbackImplBase() = default;
  virtual R Invoke(Args&&... args) const = 0;
};

template <typename F, typename R, typename... Args>
class CallbackImpl
  : public CallbackImplBase<R, Args...> {
 public:
  template<typename Functor>
  explicit CallbackImpl(Functor&& functor)
    : functor_(std::forward<Functor>(functor)) {}

  R Invoke(Args&&... args) const final {
    return functor_(std::forward<Args>(args)...);
  }

 private:
  mutable F functor_;
};

template <typename F, typename... Args>
class CallbackImpl<F, void, Args...>
  : public CallbackImplBase<void, Args...> {
 public:
  template<typename Functor>
  explicit CallbackImpl(Functor&& functor)
    : functor_(std::forward<Functor>(functor)) {}

  void Invoke(Args&&... args) const final {
    return functor_(std::forward<Args>(args)...);
  }

 private:
  mutable F functor_;
};

}  // namespace detail

template <typename T>
class Callback;

template <typename R, typename... Args>
class Callback<R(Args...)> {
 private:
  template <typename F>
  using RetunType = std::result_of_t<const F&(Args...)>;

  template <typename F>
  using IsCallback = std::is_same<typename std::decay<F>::type, Callback>;

 public:
  Callback()
    : task_impl_(nullptr) {}

  Callback(const Callback& other) = delete;
  Callback(Callback&& other) = default;

  template<typename F,
           typename = decltype((R)(std::declval<RetunType<F>>())),
           std::enable_if_t<!IsCallback<F>::value, int32_t>* = nullptr>
  Callback(F&& f)
    : task_impl_(nullptr) {
      std::cout << demangle(typeid(f).name()) << " / " << demangle(typeid(*this).name()) << std::endl;

      task_impl_ = MakeTaskImpl(std::forward<F>(f));
    }

  inline void Reset() {
    task_impl_.reset();
  }

  inline void Swap(Callback& otheer) {
    std::swap(task_impl_, otheer.task_impl_);
  }

  template <typename F>
  void Bind(F&& f) {
    task_impl_ = MakeTaskImpl(std::forward<F>(f));
  }

  R operator()(Args... args) const {
    return task_impl_->Invoke(std::forward<Args>(args)...);
  }

  template <typename F>
  Callback& operator=(F&& other) {
    task_impl_ = MakeTaskImpl(std::forward<F>(other));
    return *this;
  }

  Callback& operator=(const Callback& other) = delete;
  Callback& operator=(Callback&& other) = default;

  explicit operator bool() const {
    return static_cast<bool>(task_impl_);
  }

  friend bool operator==(std::nullptr_t, const Callback& self) {
    return !self;
  }

  friend bool operator==(const Callback& self, std::nullptr_t) {
    return !self;
  }

  friend bool operator!=(std::nullptr_t, const Callback& self) {
    return !!self;
  }

  friend bool operator!=(const Callback& self, std::nullptr_t) {
    return !!self;
  }

 private:
  template <typename T>
  using CallbackImpl_t = detail::CallbackImpl<T, R, Args...>;

  template <typename F>
  static auto MakeTaskImpl(F&& f) {
    using FunctorType = CallbackImpl_t<std::decay_t<F>>;
    return std::make_unique<FunctorType>(std::forward<F>(f));
  }

  std::unique_ptr<detail::CallbackImplBase<R, Args...>> task_impl_;
};

using TaskCallback = Callback<void()>;
using CompletionCallback = Callback<void(int32_t)>;

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_CALLBACK_TEST_CALLBACK_H_

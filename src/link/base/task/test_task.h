/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_TASK_TEST_TASK_H_
#define LINK_BASE_TASK_TEST_TASK_H_

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

template <class Sig>
struct task;

namespace details {

template <class R, class... Args>
struct task_pimpl {
  virtual R invoke(Args&&... args) const = 0;
  virtual ~task_pimpl() = default;
  virtual const std::type_info& target_type() const = 0;
};

template <class F, class R, class... Args>
struct task_pimpl_impl : task_pimpl<R, Args...> {
  F f;
  template <class Fin>
  explicit task_pimpl_impl(Fin&& fin)
    : f(std::forward<Fin>(fin)) {
  }

  R invoke(Args&&... args) const final {
    return f(std::forward<Args>(args)...);
  }

  const std::type_info& target_type() const final {
    return typeid(F);
  }
};

template <class F, class... Args>
struct task_pimpl_impl<F, void, Args...> : task_pimpl<void, Args...> {
  F f;
  template <class Fin>
  explicit task_pimpl_impl(Fin&& fin)
    : f(std::forward<Fin>(fin)) {
  }

  void invoke(Args&&... args) const final {
    f(std::forward<Args>(args)...);
  }

  const std::type_info& target_type() const final {
    return typeid(F);
  }
};

};  // namespace details

template <class R, class... Args>
struct task<R(Args...)> {
  // semi-regular:
  task() = default;
  task(task&&) = default;
  // no copy

 private:
  template <class F>
  using call_r = std::result_of_t<F const&(Args...)>;

  template <class F>
  using is_task = std::is_same<std::decay_t<F>, task>;

 public:
  // can be constructed from a callable F
  template <class F,
            // that can be invoked with Args... and converted-to-R:
            class = decltype((R)(std::declval<call_r<F>>())),
            // and is not this same type:
            std::enable_if_t<!is_task<F>{}, int>* = nullptr>

  task(F&& f)
    : m_pImpl(make_pimpl(std::forward<F>(f))) {
  }

  R operator()(Args... args) const {
    return m_pImpl->invoke(std::forward<Args>(args)...);
  }

  explicit operator bool() const {
    return static_cast<bool>(m_pImpl);
  }

  void swap(task& o) {
    std::swap(m_pImpl, o.m_pImpl);
  }

  template <class F>
  void assign(F&& f) {
    m_pImpl = make_pimpl(std::forward<F>(f));
  }

  const std::type_info& target_type() const {
    if (!*this)
      return typeid(void);
    return m_pImpl->target_type();
  }

  template <class T>
  T* target() {
    return target_impl<T>();
  }

  template <class T>
  const T* target() const {
    return target_impl<T>();
  }

  friend bool operator==(std::nullptr_t, task const& self) { return !self; }
  friend bool operator==(task const& self, std::nullptr_t) { return !self; }
  friend bool operator!=(std::nullptr_t, task const& self) { return !!self; }
  friend bool operator!=(task const& self, std::nullptr_t) { return !!self; }

 private:
  template <class T>
  using pimpl_t = details::task_pimpl_impl<T, R, Args...>;

  template <class F>
  static auto make_pimpl(F&& f) {
    using dF = std::decay_t<F>;
    using pImpl_t = pimpl_t<dF>;
    return std::make_unique<pImpl_t>(std::forward<F>(f));
  }

  std::unique_ptr<details::task_pimpl<R, Args...>> m_pImpl;

  template <class T>
  T* target_impl() const {
    return dynamic_cast<pimpl_t<T>*>(m_pImpl.get());
  }
};

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_TASK_TEST_TASK_H_

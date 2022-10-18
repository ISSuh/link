/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <link/base/callback/callback.h>

using namespace nlink;

struct FreeFunction {
    virtual int32_t Invoke(int32_t) = 0;
};

struct FreeFunctionMock : public FreeFunction {
 public:
    MOCK_METHOD1(Invoke, int32_t(int32_t));
};

FreeFunctionMock free_func_mock;

int32_t TestCallback(int32_t a) {
  return free_func_mock.Invoke(a);
}

class UserCallbacks {
  virtual void Callback1() = 0;
  virtual void Callback2(const std::string&) = 0;
  virtual int32_t Callback3(int32_t, int32_t) = 0;
};

class UserCallbacksMock : public UserCallbacks {
 public:
  MOCK_METHOD0(Callback1, void());
  MOCK_METHOD1(Callback2, void(const std::string&));
  MOCK_METHOD2(Callback3, int32_t(int32_t, int32_t));
};

TEST(Callback, make_callback) {
  {
    base::Callback<int32_t(int32_t)> callback = [](int32_t a) {
      return TestCallback(a);
    };

    EXPECT_CALL(free_func_mock, Invoke(testing::_))
      .Times(1)
      .WillOnce(::testing::Return(1));
    EXPECT_EQ(1, callback(1));
  }

  UserCallbacksMock callback_mock;

  {
    base::Callback<void()> callback = [&]() {
        callback_mock.Callback1();
      };

    EXPECT_CALL(callback_mock, Callback1());
    callback();
  }

  {
    base::Callback<void(const std::string&)> callback =
      [&](const std::string& s) {
        callback_mock.Callback2(s);
      };

    EXPECT_CALL(callback_mock, Callback2(testing::_));
    callback("test");
  }

  {
    base::Callback<int32_t(int32_t, int32_t)> callback =
      [&](int32_t a, int32_t b) -> int32_t {
        return callback_mock.Callback3(a, b);
      };

    EXPECT_CALL(callback_mock, Callback3(testing::_, testing::_))
      .Times(1)
      .WillOnce(::testing::Return(1 + 2));

    EXPECT_EQ(3, callback(1, 2));
  }
}

TEST(Callback, bind_callback) {
  UserCallbacksMock callback_mock;

  {
    base::Callback<int32_t(int32_t)> callback;
    callback.Bind(
      [](int32_t a) {
        return TestCallback(a);
      });

    EXPECT_CALL(free_func_mock, Invoke(testing::_))
      .Times(1)
      .WillOnce(::testing::Return(1));
    EXPECT_EQ(1, callback(1));
  }

  {
    base::Callback<void()> callback;
    callback.Bind(
      [&]() {
        callback_mock.Callback1();
      });

    EXPECT_CALL(callback_mock, Callback1());
    callback();
  }

  {
    base::Callback<void(const std::string&)> callback;
    callback.Bind(
      [&](const std::string& s) {
        callback_mock.Callback2(s);
      });

    EXPECT_CALL(callback_mock, Callback2(testing::_));
    callback("test");
  }

  {
    base::Callback<int32_t(int32_t, int32_t)> callback;
    callback.Bind(
      [&](int32_t a, int32_t b) -> int32_t {
        return callback_mock.Callback3(a, b);
      });

    EXPECT_CALL(callback_mock, Callback3(testing::_, testing::_))
      .Times(1)
      .WillOnce(::testing::Return(1 + 2));

    EXPECT_EQ(3, callback(1, 2));
  }
}

TEST(Callback, move_callback) {
  UserCallbacksMock callback_mock;

  {
    base::Callback<void()> callback = [&]() {
        callback_mock.Callback1();
      };

    EXPECT_CALL(callback_mock, Callback1());
    callback();

    base::Callback<void()> moved_callback = std::move(callback);
    EXPECT_FALSE(callback);
    EXPECT_TRUE(moved_callback);

    EXPECT_CALL(callback_mock, Callback1());
    moved_callback();
  }

  {
    base::Callback<void()> callback = [&]() {
      callback_mock.Callback1();
    };

    EXPECT_CALL(callback_mock, Callback1());
    callback();

    base::Callback<void()> moved_callback;
    EXPECT_FALSE(moved_callback);

    moved_callback = std::move(callback);
    EXPECT_FALSE(callback);
    EXPECT_TRUE(moved_callback);

    EXPECT_CALL(callback_mock, Callback1());
    moved_callback();
  }
}

TEST(Callback, reset_callback) {
  UserCallbacksMock callback_mock;

  {
    base::Callback<void()> callback = [&]() {
      callback_mock.Callback1();
    };

    EXPECT_CALL(callback_mock, Callback1());
    callback();

    callback.Reset();
    EXPECT_FALSE(callback);
  }
}

TEST(Callback, swap_callback) {
  UserCallbacksMock callback_mock;

  {
    base::Callback<void()> callback = [&]() {
      callback_mock.Callback1();
    };

    EXPECT_CALL(callback_mock, Callback1());
    callback();

    base::Callback<void()> moved_callback;
    EXPECT_FALSE(moved_callback);

    moved_callback.Swap(callback);
    EXPECT_FALSE(callback);
    EXPECT_TRUE(moved_callback);

    EXPECT_CALL(callback_mock, Callback1());
    moved_callback();
  }
}

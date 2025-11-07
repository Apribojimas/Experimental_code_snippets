#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <thread>
#include <tuple>

// standard thread implementation missing error proof termination
// as result was introduced jthread in C++20
// for workaround provide timed_thread clss

// timed_thread class - is std::thread with built-in start/finish tracking
class timed_thread {
public:
  // Internal wrapper to track thread execution state
  struct thread_data final {
    const char* name_ = nullptr;

    // https://medium.com/@pauljlucas/advanced-thread-safety-in-c-4cbab821356e
    std::atomic<bool> started_{false};   // set when thread function starts
    std::atomic<bool> finished_{false};  // set when thread function ends
    std::atomic<bool> joined_{false};    // set when thread is joined
    std::atomic<bool> detached_{false};  // set when thread is detached
    std::atomic<bool> exiting_{false};   // set when thread is triggered to exit

    ~thread_data() noexcept = default;
  };

  // Default constructor
  timed_thread();

  // Thread function wrapper that tracks execution state
  template <typename Fn, typename... Args>
  void thread_fn(Fn&& fn, Args&&... args) {
    // in case of thread detach access to thread internal variables not possible
    // let's hold reference to thread data
    std::shared_ptr holder(this->thread_data_);

    if (holder) { holder->started_.store(true); }
    std::invoke(std::forward<Fn>(fn), std::forward<Args>(args)...);
    if (holder) { holder->finished_.store(true); }
  }

  // Constructor that matches std::thread - accepts any callable and arguments
  template <typename Fn, typename... Args>
  explicit timed_thread(Fn&& fn, Args&&... args)
    : thread_data_(std::make_shared<thread_data>()) {
    // Launch thread with wrapper execution and forwarded arguments
    thread_ = std::make_unique<std::thread>(
      [this, fn = std::forward<Fn>(fn)](typename std::decay<Args>::type... args) mutable {
          this->thread_fn(std::move(fn), std::forward<decltype(args)>(args)...);
      },
      std::forward<Args>(args)...);
  }

  // Move constructor
  timed_thread(timed_thread&& other) noexcept;

  // Move assignment
  timed_thread& operator=(timed_thread&& other) noexcept;

  // Delete copy operations
  timed_thread(const timed_thread&) = delete;
  timed_thread& operator=(const timed_thread&) = delete;

  // Destructor
  virtual ~timed_thread();

  // Waits until thread finished its execution or timeout occurred
  // Using thread_data provided information
  // param sleep_period: time period to sleep between checks of flags
  // param timeout: maximum time to wait until function returns
  // return true if timeout occurred
  bool timed_join(
    std::chrono::milliseconds sleep_period = std::chrono::milliseconds(10),  // recommended > configTICK_RATE_HZ
    std::chrono::milliseconds timeout = std::chrono::milliseconds(3000));

  // Helper function: join/detach thread based on is_running flag
  void finalize(bool is_running);

  // Helper function: join/detach thread based on internal flags
  void finalize();

  // Join the thread
  void join();

  // Detach the thread
  void detach();

  // Check if joinable
  [[nodiscard]] bool joinable() const noexcept;

  // Get thread id
  [[nodiscard]] std::thread::id get_id() const noexcept;

  // Check if thread has started
  [[nodiscard]] bool is_started() const;

  // Check if thread still running
  [[nodiscard]] bool is_running() const;

  // Check if thread has finished
  [[nodiscard]] bool is_finished() const;

  // Check if thread has joined
  [[nodiscard]] bool is_joined() const;

  // Check if thread has detached
  [[nodiscard]] bool is_detached() const;

  // Check if exiting flag is set
  [[nodiscard]] bool is_exiting() const;

  // Set exiting flag
  void set_exiting(bool exit) noexcept;

  // thread name functionality
  [[nodiscard]] const char* get_name() const noexcept;
  void set_name(const char* name) noexcept;

  // Dump thread state as string for debug purposes
  [[nodiscard]] std::string dump() const;

 protected:
  std::unique_ptr<std::thread> thread_;
  std::shared_ptr<thread_data> thread_data_;
};

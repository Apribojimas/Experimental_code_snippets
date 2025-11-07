#include "pt.h"

#include <string>

// Default constructor
timed_thread::timed_thread()
: thread_data_(std::make_shared<thread_data>()) {
}

// Move constructor
timed_thread::timed_thread(timed_thread&& other) noexcept
  : thread_(std::move(other.thread_)),
  thread_data_(std::move(other.thread_data_)) {
}

// Move assignment
timed_thread& timed_thread::operator=(timed_thread&& other) noexcept {
  if (this != &other) {
    // Clean up current thread if it exists and is joinable
    if (thread_ && thread_->joinable()) {
      thread_->join();
      if (thread_data_) {
        thread_data_->joined_.store(true);
      }
    }
    // Move resources from other
    thread_ = std::move(other.thread_);
    thread_data_ = std::move(other.thread_data_);
  }
  return *this;
}

// Destructor
timed_thread::~timed_thread() {
  // For debug purposes only
  // printf(dump().c_str());

  if (!is_joined() && !is_detached()) {
    // exceptional endding, recommend avoid situation vithout proper join/detach in code
    timed_join();
  }

  thread_.reset();
  thread_data_.reset();
}

// Waits until thread finished its execution or timeout occurred
// Using thread_data provided information
// param sleep_period: time period to sleep between checks of flags
// param timeout: maximum time to wait until function returns
// return true if timeout occurred
bool timed_thread::timed_join(std::chrono::milliseconds sleep_period,
  std::chrono::milliseconds timeout) {
  if (joinable() && !is_joined() && !is_detached()) {
    detach();  // detach to avoid blocking in destructor if not joined
    set_exiting(true);  // signal thread to exit if it checks this flag

    // thread detach and still running
    auto start_time = std::chrono::steady_clock::now();
    while (!is_finished()) {
      auto elapsed_time = std::chrono::steady_clock::now() - start_time;
      if (elapsed_time >= timeout) {
        // timeout occurred
        // Future implementation task: implement logging here or assert. Always keep breakpoint for debug.
        return true;
      }
      std::this_thread::sleep_for(sleep_period);
    }
  }
  return false;
}

// Helper function: join/detach thread based on is_running flag
void timed_thread::finalize(bool is_running) {
  if (is_running) {
    join();  // still running, join
  } else {
    detach();  // already in shutdown, detach
  }
}

// Helper function: join/detach thread based on internal flags flag
void timed_thread::finalize() {
  // Thread lifecycle decision logic:
  // - If thread was just created but hasn't started yet - so join it
  // - If thread has started but hasn't finished yet - it's still running, so join it
  // - If thread has already finished  detach it
  finalize(!is_finished());
}

// Join the thread
void timed_thread::join() {
  if (joinable()) {
    if (thread_data_) {
      thread_data_->joined_.store(true);
    }
    try {
      thread_->join();
    } catch (...) {}  // do nothing, swallow exceptions
  }
}

// Detach the thread
void timed_thread::detach() {
  if (joinable()) {
    if (thread_data_) {
      thread_data_->detached_.store(true);
    }
    try {
      thread_->detach();
    } catch (...) {}  // do nothing, swallow exceptions
  }
}

// Check if joinable
bool timed_thread::joinable() const noexcept {
  // in case of no thread, not joinable
  return thread_ ? thread_->joinable() : false;
}

// Get thread id
std::thread::id timed_thread::get_id() const noexcept {
  return thread_ ? thread_->get_id() : std::thread::id();
}

// Check if thread has started
bool timed_thread::is_started() const {
  return thread_data_ ? thread_data_->started_.load() : false;
}

// Check if thread still running
bool timed_thread::is_running() const {
  return is_started() && !is_finished();
}

// Check if thread has finished
bool timed_thread::is_finished() const {
  // in case of no wrapper, consider finished
  return thread_data_ ? thread_data_->finished_.load() : true;
}

// Check if thread has joined
bool timed_thread::is_joined() const {
  return thread_data_ ? thread_data_->joined_.load() : false;
}

// Check if thread has detached
bool timed_thread::is_detached() const {
  return thread_data_ ? thread_data_->detached_.load() : false;
}

// Check if exit flag is set
[[nodiscard]] bool timed_thread::is_exiting() const {
  return thread_data_ ? thread_data_->exiting_.load() : true;
}

// Set exit flag
void timed_thread::set_exiting(bool exit) noexcept {
  if (thread_data_) {
    thread_data_->exiting_.store(exit);
  }
}

// thread name functionality
const char* timed_thread::get_name() const noexcept {
  return thread_data_ ? thread_data_->name_ : nullptr;
}

void timed_thread::set_name(const char* name) noexcept {
  if (thread_data_) {
    thread_data_->name_ = name;
  }
}

// Dump thread state as string for debug purposes
std::string timed_thread::dump() const {
  char buffer[256] = {};
  std::snprintf(buffer, sizeof(buffer),
    "Thread Name: '%s', Running: %d, Started: %d, Finished: %d, Joinable: %d, Joined: %d, Detached: %d",
    get_name() ? get_name() : "Unnamed",
    is_running(),
    is_started(),
    is_finished(),
    joinable(),
    is_joined(),
    is_detached());
return std::string(buffer);
}

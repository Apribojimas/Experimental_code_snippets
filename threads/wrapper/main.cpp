#include <deque>
#include <iostream>
#include <mutex>
#include <string>

#include "pt.h"

// Helper function to create timed_thread with callback
template <typename WorkFunc, typename CallbackFunc>
timed_thread make_priority_thread_with_callback(WorkFunc&& work,
                                                CallbackFunc&& callback) {
  // Create a combined functor
  struct CombinedWork {
    typename std::decay<WorkFunc>::type work_func;
    typename std::decay<CallbackFunc>::type callback_func;

    void operator()() {
      work_func();
      callback_func();
    }
  };

  return timed_thread(CombinedWork{std::forward<WorkFunc>(work),
                                   std::forward<CallbackFunc>(callback)});
}

// Mutex for thread-safe console output
std::mutex console_mutex;

// Thread-safe print function
void safe_print(const std::string& message) {
  std::lock_guard<std::mutex> lock(console_mutex);
  std::cout << message << std::endl;
}

// Example work functions
void do_work(int thread_id, int duration_ms) {
  safe_print("Thread " + std::to_string(thread_id) + " started");
  std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
  safe_print("Thread " + std::to_string(thread_id) + " work completed");
}

// Example: Work function that returns a value (captured in closure)
int calculate_sum(int n) {
  int sum = 0;
  for (int i = 1; i <= n; ++i) {
    sum += i;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  return sum;
}

class ThreadRunCallback {
 public:
  void Run(const std::string& message) {
    safe_print("ThreadRunCallback: Starting work...");
    safe_print(message);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    safe_print("ThreadRunCallback: Work completed");
  }
};

class ThreadAndCallback {
 public:
  ThreadAndCallback(int id)
      : thread(&ThreadAndCallback::ThreadFunction, this, id) {}

  void ThreadFunction(int id) {
    safe_print("ThreadAndCallback ThreadFunction " + std::to_string(id) +
               ": Starting work...");
    while (!thread.is_exiting()) {
      safe_print("ThreadAndCallback ThreadFunction " + std::to_string(id) +
                 ": Working...");
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    safe_print("ThreadAndCallback ThreadFunction " + std::to_string(id) +
               ": Work completed");
  }

  timed_thread thread;
};

int main() {
  safe_print("=== timed_thread with Start/Finish Tracking ===\n");

  // Example 0: constructing and destructing
  safe_print("Example 0: constructing and destructing\n");
  {
    {
      timed_thread thread(safe_print, "Example 0: Print ...");
      thread.finalize();
      safe_print("Thread state dump: " + thread.dump());
    }
    {  // default
      timed_thread thread;
    }
  }

  // Example 1: Using timed_thread just like std::thread
  std::deque<timed_thread> threads;
  safe_print("Example 1: timed_thread with std::thread-style construction\n");

  // Just like std::thread - pass function and arguments
  threads.push_back(timed_thread(do_work, 1, 300));
  threads.push_back(timed_thread(do_work, 2, 600));
  threads.push_back(timed_thread(do_work, 3, 900));

  // Example 2: Tracking thread state
  safe_print("\nExample 2: Tracking thread start/finish state\n");

  timed_thread tracked_thread([]() {
    safe_print("Tracked thread: Starting work...");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    safe_print("Tracked thread: Work completed");
  });

  // Check if started (should be true almost immediately)
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  safe_print("Is tracked thread started? " +
             std::string(tracked_thread.is_started() ? "YES" : "NO"));
  safe_print("Is tracked thread finished? " +
             std::string(tracked_thread.is_finished() ? "YES" : "NO"));

  // Wait a bit and check again
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  safe_print("After 500ms - Is finished? " +
             std::string(tracked_thread.is_finished() ? "YES" : "NO"));
  safe_print("Thread state dump: " + tracked_thread.dump());

  tracked_thread.join();
  safe_print("After join - Is finished? " +
             std::string(tracked_thread.is_finished() ? "YES" : "NO"));

  tracked_thread.set_name("tracked_thread");
  safe_print("Thread state after naming: " + tracked_thread.dump());

  // Example 3: Using helper function for callback support
  safe_print("\nExample 3: timed_thread with callbacks\n");

  for (int i = 4; i <= 6; ++i) {
    threads.push_back(make_priority_thread_with_callback(
        // Work function
        [i]() { do_work(i, i * 200); },
        // Completion callback
        [i]() {
          safe_print(">>> CALLBACK: timed_thread " + std::to_string(i) +
                     " finished!");
        }));
  }

  // Example 4: Lambda with std::thread-style construction
  safe_print("\nExample 4: timed_thread with lambda (std::thread style)\n");

  threads.push_back(timed_thread([]() {
    safe_print("Thread 7: Lambda execution started");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    safe_print("Thread 7: Lambda execution completed");
  }));

  // Example 5: Callback with result capture
  safe_print("\nExample 5: timed_thread with result capture\n");

  int result = 0;
  threads.push_back(make_priority_thread_with_callback(
      // Work function
      [&result]() {
        safe_print("Thread 8: Calculating sum of 1..50");
        result = calculate_sum(50);
        safe_print("Thread 8: Calculation complete");
      },
      // Callback
      [&result]() {
        safe_print(">>> CALLBACK: Thread 8 finished!");
        safe_print(" Computed result: " + std::to_string(result));
      }));

  // Example 6: Function objects with std::thread-style construction
  safe_print("\nExample 6: timed_thread with function objects\n");

  struct WorkTask {
    int id;
    void operator()() const {
      safe_print("Thread " + std::to_string(id) + ": Executing task");
      std::this_thread::sleep_for(std::chrono::milliseconds(400));
      safe_print("Thread " + std::to_string(id) + ": Task execution done");
    }
  };

  threads.push_back(timed_thread(WorkTask{9}));

  // Example 7: Callback with function objects
  safe_print("\nExample 7: timed_thread with function object callbacks\n");

  struct CompletionHandler {
    int id;
    void operator()() const {
      safe_print(">>> CALLBACK: Thread " + std::to_string(id) +
                 " completion handler invoked!");
    }
  };

  threads.push_back(
      make_priority_thread_with_callback(WorkTask{10}, CompletionHandler{10}));

  // Example 8: Monitoring multiple threads
  safe_print("\nExample 8: Monitoring thread completion status\n");

  // Give threads some time to start
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  int started_count = 0;
  int finished_count = 0;
  for (const auto& t : threads) {
    if (t.is_started()) started_count++;
    if (t.is_finished()) finished_count++;
  }

  safe_print("Status check: " + std::to_string(started_count) + " started, " +
             std::to_string(finished_count) + " finished");

  // Wait for all threads to complete
  safe_print("\nMain thread: Waiting for all timed_threads to finish...\n");

  for (auto& t : threads) {
    t.join();
  }

  // Final status check
  started_count = 0;
  finished_count = 0;
  for (const auto& t : threads) {
    if (t.is_started()) started_count++;
    if (t.is_finished()) finished_count++;
  }

  safe_print("Final status: " + std::to_string(started_count) + " started, " +
             std::to_string(finished_count) + " finished");

  // Example 9: Class method as thread function
  ThreadRunCallback callbackObj;
  safe_print("\nExample 9: timed_thread with class method as function\n");
  timed_thread class_method_thread(&ThreadRunCallback::Run, &callbackObj,
                                   "Extra param test");
  class_method_thread.join();

  // Example 10: lambda with capture, reassigned to timed_thread
  safe_print("\nExample 10: timed_thread with move assignment\n");
  timed_thread capture_thread([=]() {
    safe_print("capture_thread: Starting work...");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    safe_print("capture_thread: Work completed");
  });

  // Move the thread to a new variable
  timed_thread assigned_thread = std::move(capture_thread);

  // Original thread should no longer be joinable after move
  safe_print("Original thread joinable after move: " +
             std::string(capture_thread.joinable() ? "YES" : "NO"));
  safe_print("Assigned thread joinable: " +
             std::string(assigned_thread.joinable() ? "YES" : "NO"));

  // Join the assigned thread
  assigned_thread.join();
  safe_print("Assigned thread finished: " +
             std::string(assigned_thread.is_finished() ? "YES" : "NO"));

  // Example 11: empty_thread
  {
    safe_print("\nExample 11: Empty timed_thread (default constructor)\n");
    timed_thread empty_thread;
  }

  // Example 12: Using thread_fn directly (advanced usage)
  {
    ThreadRunCallback callbackObj2;
    safe_print(
        "\nExample 12: Direct use of thread_fn (manual thread management)\n");

    // Create a timed_thread object and use its thread_fn member function
    timed_thread manual_wrapper;

    manual_wrapper.thread_fn(&ThreadRunCallback::Run, &callbackObj2,
                             "Manual thread_fn usage");

    // Create a thread that calls the member function
    std::thread manual_thread(&ThreadRunCallback::Run, &callbackObj2,
                              "Constructor thread_fn usage");

    // Wait for it to finish
    manual_thread.join();

    safe_print("Manual thread started: " +
               std::string(manual_wrapper.is_started() ? "YES" : "NO"));
    safe_print("Manual thread finished: " +
               std::string(manual_wrapper.is_finished() ? "YES" : "NO"));
  }

  // Example 13: detach cleanup
  {
    safe_print("\nExample 13: Thread detach with cleanup wait\n");

    timed_thread detach_cleanup([=]() {
      safe_print("detach_cleanup: Starting work...");
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      safe_print("detach_cleanup: Work completed");
    });
    detach_cleanup.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  // Example 14: destructor cleanup
  {
    safe_print("\nExample 14: Automatic cleanup in destructor (timed_join)\n");

    timed_thread destructor_cleanup([=]() {
      safe_print("destructor_cleanup: Starting work...");
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      safe_print("destructor_cleanup: Work completed");
    });
  }

  // Example 15: finalize method
  {
    safe_print(
        "\nExample 15: Using finalize method for conditional join/detach\n");

    timed_thread finalize_thread([=]() {
      safe_print("finalize_thread: Starting work...");
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      safe_print("finalize_thread: Work completed");
    });
    finalize_thread.finalize(!finalize_thread.is_finished());  // join
  }

  // Example 16: Thread within a class
  {
    safe_print("\nExample 16: timed_thread as class member\n");
    ThreadAndCallback tac0(0);
    std::this_thread::sleep_for(std::chrono::milliseconds(700));
    tac0.thread.set_exiting(true);  // signal thread to exit
    tac0.thread.join();

    ThreadAndCallback tac1(1);
    std::this_thread::sleep_for(std::chrono::milliseconds(700));
    tac1.thread.timed_join();
  }

  // Example 17: Testing is_running() in different states
  {
    safe_print("\nExample 17: Testing is_running() across thread lifecycle\n");

    // Test 1: Thread not started yet (default constructed)
    timed_thread not_started;
    safe_print("Default thread is_running: " +
               std::string(not_started.is_running() ? "YES" : "NO") +
               " (expected: NO)");

    // Test 2: Thread just created but may not have started
    timed_thread just_created(
        []() { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
    safe_print("Just created is_running: " +
               std::string(just_created.is_running() ? "YES" : "NO") +
               " (may vary)");

    // Test 3: Thread definitely running
    timed_thread running_thread(
        []() { std::this_thread::sleep_for(std::chrono::milliseconds(500)); });
    std::this_thread::sleep_for(
        std::chrono::milliseconds(50));  // Give it time to start
    safe_print("Running thread is_running: " +
               std::string(running_thread.is_running() ? "YES" : "NO") +
               " (expected: YES)");
    safe_print("Running thread state: " + running_thread.dump());

    // Test 4: Thread finished
    running_thread.join();
    safe_print("Finished thread is_running: " +
               std::string(running_thread.is_running() ? "YES" : "NO") +
               " (expected: NO)");
    safe_print("Finished thread state: " + running_thread.dump());

    // Test 5: Thread detached
    timed_thread detached_thread(
        []() { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    detached_thread.detach();
    safe_print("Detached thread is_running: " +
               std::string(detached_thread.is_running() ? "YES" : "NO"));
    std::this_thread::sleep_for(
        std::chrono::milliseconds(200));  // Wait for it to finish
  }

  // Example 18: Testing finalize() with different states
  {
    safe_print(
        "\nExample 18: Testing finalize() behavior in different scenarios\n");

    // Test 1: finalize() on not-yet-started thread (should detach)
    {
      timed_thread not_started_thread;
      safe_print("Before finalize - not started: " + not_started_thread.dump());
      not_started_thread.finalize();
      safe_print("After finalize - not started: " + not_started_thread.dump());
    }

    // Test 2: finalize() on running thread (should join)
    {
      timed_thread running_thread([]() {
        safe_print("finalize test: Thread executing...");
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        safe_print("finalize test: Thread completed");
      });
      std::this_thread::sleep_for(
          std::chrono::milliseconds(50));  // Ensure it's running
      safe_print("Before finalize - running: " + running_thread.dump());
      running_thread.finalize();  // Should join
      safe_print("After finalize - running: " + running_thread.dump());
    }

    // Test 3: finalize() on finished thread (should detach)
    {
      timed_thread finished_thread(
          []() { safe_print("finalize test: Quick thread"); });
      std::this_thread::sleep_for(
          std::chrono::milliseconds(100));  // Wait for completion
      safe_print("Before finalize - finished: " + finished_thread.dump());
      finished_thread.finalize();  // Should detach
      safe_print("After finalize - finished: " + finished_thread.dump());
    }

    // Test 4: finalize(true) - explicit join
    {
      timed_thread explicit_join_thread([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
      });
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      safe_print("Before finalize(true): " + explicit_join_thread.dump());
      explicit_join_thread.finalize(true);  // Explicit join
      safe_print("After finalize(true): " + explicit_join_thread.dump());
    }

    // Test 5: finalize(false) - explicit detach
    {
      timed_thread explicit_detach_thread([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
      });
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      safe_print("Before finalize(false): " + explicit_detach_thread.dump());
      explicit_detach_thread.finalize(false);  // Explicit detach
      safe_print("After finalize(false): " + explicit_detach_thread.dump());
      std::this_thread::sleep_for(
          std::chrono::milliseconds(200));  // Wait for detached thread
    }
  }

  // Example 19: Testing is_running() with is_exiting flag
  {
    safe_print("\nExample 19: Testing is_running() with exiting flag\n");

    timed_thread exit_aware_thread(
        [](timed_thread* self) {
          safe_print("exit_aware: Started");
          int count = 0;
          while (!self->is_exiting() && count < 10) {
            safe_print("exit_aware: Iteration " + std::to_string(count++));
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
          }
          safe_print("exit_aware: Exiting (is_exiting=" +
                     std::string(self->is_exiting() ? "YES" : "NO") + ")");
        },
        &exit_aware_thread);

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    safe_print("Before exit signal - is_running: " +
               std::string(exit_aware_thread.is_running() ? "YES" : "NO"));

    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    exit_aware_thread.set_exiting(true);
    safe_print("After exit signal - is_running: " +
               std::string(exit_aware_thread.is_running() ? "YES" : "NO"));

    exit_aware_thread.join();
    safe_print("After join - is_running: " +
               std::string(exit_aware_thread.is_running() ? "YES" : "NO"));
    safe_print("Final state: " + exit_aware_thread.dump());
  }

  // Example 20: Edge cases - finalize on already joined/detached threads
  {
    safe_print(
        "\nExample 20: Edge cases - finalize on already joined/detached "
        "threads\n");

    // Test 1: finalize() after manual join
    {
      timed_thread already_joined([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      });
      already_joined.join();
      safe_print("After manual join: " + already_joined.dump());
      already_joined.finalize();  // Should be safe to call
      safe_print("After finalize on joined thread: " + already_joined.dump());
    }

    // Test 2: finalize() after manual detach
    {
      timed_thread already_detached([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      });
      already_detached.detach();
      safe_print("After manual detach: " + already_detached.dump());
      already_detached.finalize();  // Should be safe to call
      safe_print("After finalize on detached thread: " +
                 already_detached.dump());
      std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    // Test 3: Multiple finalize() calls
    {
      timed_thread multi_finalize([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      });
      std::this_thread::sleep_for(std::chrono::milliseconds(150));
      safe_print("First finalize: " + multi_finalize.dump());
      multi_finalize.finalize();
      safe_print("After first finalize: " + multi_finalize.dump());
      multi_finalize.finalize();  // Second call should be safe
      safe_print("After second finalize: " + multi_finalize.dump());
    }
  }

  // Additional test cases for timed_join() - to be added to main.cpp after Example 20

    // Example 21: Testing timed_join() - successful completion
  {
    safe_print("\nExample 21: timed_join() - successful completion\n");

    timed_thread quick_thread([]() {
      safe_print("Quick thread running...");
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      });

    bool timeout = quick_thread.timed_join(std::chrono::milliseconds(10), std::chrono::milliseconds(500));
    safe_print("Result: " + std::string(timeout ? "TIMEOUT" : "SUCCESS"));
  }

  // Example 22: Testing timed_join() - timeout scenario
  {
    safe_print("\nExample 22: timed_join() - timeout\n");

    timed_thread long_thread([]() {
      std::this_thread::sleep_for(std::chrono::milliseconds(2000));
      });

    bool timeout = long_thread.timed_join(std::chrono::milliseconds(10), std::chrono::milliseconds(300));
    safe_print("Result: " + std::string(timeout ? "TIMEOUT" : "SUCCESS"));
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
  }

  // Example 23: Testing timed_join() - edge cases
  {
    safe_print("\nExample 23: timed_join() - edge cases\n");

    timed_thread default_thread;
    bool timeout1 = default_thread.timed_join();
    safe_print("Default: " + std::string(timeout1 ? "TIMEOUT" : "SUCCESS"));

    timed_thread already_joined([]() { std::this_thread::sleep_for(std::chrono::milliseconds(50)); });
    already_joined.join();
    bool timeout2 = already_joined.timed_join();
    safe_print("Already joined: " + std::string(timeout2 ? "TIMEOUT" : "SUCCESS"));
  }


  safe_print("\n=== All timed_threads completed ===");
  return 0;
}
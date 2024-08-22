# Thread pool requirements
## Scope of functionality
* Create/Destroy
* Start/Stop
* Suspend(Pause)/Resume
* Shutdown/Terminate
* Timeout/Postpone/Interval
* Duration of last execution
* Execution callback must support PRE/POST/ERROR/CALLBACK interfaces
* 0 execeptions
* Ideal for:
  - Actor framework mailbox
  - Event bus dispatcher
  - Timers/schedulers
  - Real Time task?
  - Support task stealing for better workload
  - Support threads priorities
  - MSVC & windows only? Embedded (dynamic memory allocation)?
  - Dependence only to STL library, google test framework. No BOOST or magic libraries
  - C++17, coroutines?

## Sources of ideas
* [muduo](https://github.com/chenshuo/muduo) - Event-driven network library for multi-threaded Linux server in C++11
    - [WeakCallback](https://github.com/chenshuo/muduo/blob/master/muduo/base/WeakCallback.h) - 
* [ThreadPool](https://github.com/progschj/ThreadPool) - A simple C++11 Thread Pool implementation
* [dbg(…)](https://github.com/sharkdp/dbg-macro) - A dbg(…) macro for C++
* [BS::thread_pool](https://github.com/bshoshany/thread-pool) - BS::thread_pool: a fast, lightweight, and easy-to-use C++17 thread pool library
* [Async++](https://github.com/Amanieu/asyncplusplus) - Async++ concurrency framework for C++11
   - [work_steal_queue](https://github.com/Amanieu/asyncplusplus/blob/master/src/work_steal_queue.h) - Chase-Lev work stealing deque
* [eventpp](https://github.com/wqking/eventpp) - Event Dispatcher and callback list for C++
    - [EventDispatcherBase](https://github.com/wqking/eventpp/blob/master/include/eventpp/eventdispatcher.h) - dispatcher interface
* [workspace](https://github.com/CodingHanYa/workspace) - workspace is a lightweight asynchronous execution framework based on C++11, which supports: asynchronous concurrent execution of general tasks, priority task scheduling, adaptive dynamic thread pools, efficient static thread pools, and exception handling mechanisms.
* [Scheduling](https://github.com/dpuyda/scheduling) - A simple and fast library allowing to run async tasks and execute task graphs.
* [concurrencpp](https://github.com/David-Haim-zz/concurrencpp) - timer implementation
* [SmartThreadPool](https://github.com/leosocy/SmartThreadPool) - priority task pools
* [reactor](https://github.com/fantasy-peak/reactor) - C++17 and reactor mode task/timer executor
* [ThreadPoolLib](https://github.com/geru-scotland/ThreadPoolLib) - A simple but efficient C++ thread/worker pool library for asynchronous task management.

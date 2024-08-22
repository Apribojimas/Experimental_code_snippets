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

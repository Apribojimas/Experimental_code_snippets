# Thread pool requirements
## Scope of functionality
* Create/Destroy (ThreadPool/Thread/ThreadQueue/Task(job/Context))
* Start/Stop (ThreadPool/Thread/Task(job))
* Suspend(Pause)/Resume (ThreadPool/Thread/Task(job))
* Shutdown/Terminate (ThreadPool/Thread/Task(job))
* Timeout/Postpone/Interval (Task(job)) -> timer or IO
* Duration of last execution (Task(job))
* Execution callback must support PRE/POST/ERROR/CALLBACK/READY interfaces (Task(job))
* 0 execeptions
* Asyn/awaitable single execution
* Current state: Uninitialized/Initialized/Running/Waiting/Stopped/Paused/Timeout/Shutdown/
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
* [ThreadPool](https://github.com/progschj/ThreadPool), [C++ Thread Pool](https://github.com/f-squirrel/thread_pool), [threadpool.hpp](https://github.com/leiless/threadpool.hpp), [ThreadPoolC++11](https://github.com/xingyuuchen/ThreadPool), [intelRSD](https://github.com/intel/intelRSD/blob/master/PSME/common/agent-framework/include/agent-framework/threading/threadpool.hpp) - A simple C++11 Thread Pool implementation
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
* [CXX11ThreadPool](https://github.com/upider/CXX11ThreadPool) - Module/architecture solution?
* [quickpool](https://github.com/tnagler/quickpool) - solution for free functions, memory aligned hints
* [madthreading](https://github.com/jrmadsen/madthreading) - A low-overhead, task-based threading API using a thread-pool of C++11 threads
    - [task](https://github.com/jrmadsen/madthreading/blob/master/madthreading/threading/task/task.hh) - solution for keeping task results
* [cyan](https://github.com/sayan-chaliha/cyan) - A lock-free, wait-free, event-driven, cross-platform, thread pool-backed asynchronous execution library in C++. 
* [Work-Stealing Queue](https://github.com/taskflow/work-stealing-queue) - A fast work-stealing queue template in C++ or Lock-free unbounded single-producer multiple-consumer queue.
* [enkiTS](https://github.com/dougbinks/enkiTS) - A permissively licensed C and C++ Task Scheduler for creating parallel programs. Requires C++11 support.
* [transwarp](https://github.com/bloomen/transwarp) - Contains of requested functionality
* [Yaaf](https://github.com/lysevi/yaaf/blob/master/libyaaf/utils/async/thread_manager.h) - Thread manager -> Thread pool
* [TaskManager](https://github.com/Tastyep/TaskManager) - Simple closest implementation
* [ThrottlingThreadPool](https://github.com/LoganEvans/ThrottlingThreadPool) - Intresting
* [DFS-IoT](https://github.com/Rudnik-Ilia/DFS-IoT) - Priority queues
* [cpp-taskflow](https://github.com/artivis/cpp-taskflow) - Earlier fork of "TaskFlow", contains work stealing thread pool

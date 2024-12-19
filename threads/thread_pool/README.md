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
  - pure C++17 only. Most embedded don't support coroutines, conditional variables, future, promises, etc?

## Sources of ideas
* [Scheduling](https://github.com/dpuyda/scheduling/blob/main/include/scheduling/scheduling.hpp) - A simple and fast library allowing to run async tasks and execute task graphs.
* [nmcclatchey](https://github.com/nmcclatchey/ThreadPool) - Portable (C++11) low-overhead concurrent task scheduling for fine-grained concurrency.
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
* [gws-polarlight-sunshine](https://github.com/Tencent/gws-polarlight-sunshine/blob/main/src/thread_pool.h) - Allow threads to execute unhindered while keeping full control over the threads.
* [mango](https://github.com/tksuoran/erhe/blob/main/src/mango/include/mango/core/thread.hpp) - The tasks have no dependency to each other and can be executed in any order.
* [koios](https://github.com/JPewterschmidt/koios) - A C++ coroutine library
* [1261385937](https://github.com/1261385937/thread_pool) - very high performance, lock free, roundRobin dispatching, steal available
* [task-thread-pool](https://github.com/alugowski/task-thread-pool/blob/main/include/task_thread_pool.hpp) - Fast and lightweight thread pool for C++11 and newer.
* [C++ Timeouts](https://github.com/eglimi/cpptime/blob/master/cpptime.h) - A portable, pure C++11 timer component.
* [rabbit hole](https://github.com/lukasino1214/graphics-programming-rabbit-hole/blob/master/src/threadpool.hpp) - rabbit hole
* [muu](https://github.com/marzer/muu/blob/master/include/muu/thread_pool.h) - stateless vs state callbacks
## Libs
* [Senders - A Standard Model for Asynchronous Execution in C++](https://github.com/NVIDIA/stdexec) - `std::execution`, the proposed C++ framework for asynchronous and parallel programming.
* [reactor](https://github.com/fantasy-peak/reactor) - C++17 and reactor mode task/timer executor
* [aco::thread_pool](https://github.com/antoniocoratelli/thread-pool/blob/main/library/include/aco/thread_pool.hpp) - C++17 thread-pool-based task scheduler.
* [STLab](https://github.com/stlab/libraries) - Concurrency (C++20) and fork [stlab_libraries](https://github.com/jeffamstutz/stlab_libraries) for C++14
* [Fiber Tasking Lib](https://github.com/RichieSams/FiberTaskingLib) - A library for enabling task-based multi-threading. It allows execution of task graphs with arbitrary dependencies.
* [concurrencpp](https://github.com/David-Haim/concurrencpp) - Modern concurrency for C++. Tasks, executors, timers and C++20 coroutines to rule them all
* [TaskManager](https://github.com/Tastyep/TaskManager) - A C++14 Task Manager / Scheduler
* [psched](https://github.com/p-ranav/psched) - Priority-based Task Scheduling for Modern C++
* [ssTs](https://github.com/StefanoLusardi/task_scheduler) - Small & Simple Task Scheduler for C++17
* [SuperGlue](https://github.com/tillenius/superglue) - SuperGlue -- A C++ Library for Data-Dependency Driven Task Parallelism
* [Taskflow](https://github.com/taskflow/taskflow) - A General-purpose Task-parallel Programming System using Modern C++
* [poolSTL](https://github.com/alugowski/poolSTL) - Light and self-contained implementation of C++17 parallel algorithms.
* [workspace](https://github.com/CodingHanYa/workspace) - workspace is a lightweight asynchronous execution framework based on C++11, which supports: asynchronous concurrent
* [antoniocoratelli](https://github.com/antoniocoratelli/thread-pool/blob/main/library/include/aco/thread_pool.hpp) - A minimalist implementation of a C++17 thread-pool-based task scheduler with task-stealing, in around 200 lines of header-only code with no external dependencies.
* [SimpleThreadPools](https://github.com/ShinuziKyura/SimpleThreadPools) - intresting task&pack approach
* [task-thread-pool](https://github.com/alugowski/task-thread-pool) - Fast and lightweight thread pool for C++11 and newer.
* [Thread System Project](https://github.com/kcenon/thread_system) - A modern C++ threading framework offering robust thread management via thread_base class, real-time multi-threaded logging system, thread pool with priority queue support, compatibility with std::jthread (C++20) and std::thread, and comprehensive error handling mechanisms.
* [Beehive](https://github.com/egranata/beehive) - A C++ thread pool manager
* [Multithreading_Cpp](https://github.com/trololo23/Multithreading_Cpp) - Executors and other Structures

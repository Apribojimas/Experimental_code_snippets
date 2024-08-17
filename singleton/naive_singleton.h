// Singleton helper class definition ------------------------
// Avoid unnecessary mutex locking in single thread application
struct NoLockingMutex {
  inline void lock() {}
  inline void unlock() {}
  inline bool try_lock() { return true; }
};

// Singleton class definition ------------------------
// Singleton is thread safe and support life cycle
template <typename T, typename M = NoLockingMutex>
class Singleton {
public:
  Singleton() noexcept = default;
  virtual ~Singleton();
  // do not copy
  Singleton(const Singleton&) noexcept = delete;
  Singleton& operator=(const Singleton&) noexcept = delete;
  // do not move
  Singleton(Singleton&&) noexcept = delete;
  Singleton& operator=(Singleton&&) noexcept = delete;

  // exclusive instance [re]create
  template<typename... Ts>
  static void Create(Ts&&... args);

  // destroy created instance
  static void Release();

  // retrieve pointer to instance and create if neccesary
  template<typename... Ts>
  static T* GetInstance(Ts&&... args);

private:
  static M lock_;  // in case of multithreading use std::mutex
  static std::unique_ptr<T> instance_;
};

// Singleton<T> implementation --------------------------
template <typename T, typename M>
Singleton<T, M>::~Singleton() {
  Release();
}

template <typename T, typename M>
template<typename... Ts>
static void Singleton<T, M>::Create(Ts&&... args) {
  if (instance_ != nullptr) {
    Release();
  }

  if (instance_ == nullptr) {
    lock_.lock();
    instance_ = std::make_unique<T>(std::forward<Ts>(args)...);
    lock_.unlock();
  }
}

template <typename T, typename M>
void Singleton<T, M>::Release() {
  if (instance_ != nullptr) {
    lock_.lock();
    // Double-Checked Locking Pattern (DCLP)
    if (instance_ != nullptr) {
      instance_.reset();
    }
    lock_.unlock();
  }
}

template <typename T, typename M>
template<typename... Ts>
T* Singleton<T, M>::GetInstance(Ts&&... args) {
  if (instance_ == nullptr) {
    Create(std::forward<Ts>(args)...);
  }
  return instance_.get();
}

template <typename T, typename M> M Singleton<T, M>::lock_;
template <typename T, typename M> std::unique_ptr<T> Singleton<T, M>::instance_;


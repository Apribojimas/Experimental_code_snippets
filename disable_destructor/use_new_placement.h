
template <class T> class Indestructible {
    alignas(T) unsigned char data_[sizeof(T)];  // using 'new placement' trick
  public:
    template <class... Ts> Indestructible(Ts&&... ts) noexcept { new (data_) T(std::forward<Ts>(ts)...); }
    ~Indestructible() noexcept = default;

    const T& value() const { return *reinterpret_cast<T*>(data_); }
    T& value() { return *reinterpret_cast<T*>(data_); }
    const T* get() const { return reinterpret_cast<T*>(data_); }
    T* get() { return reinterpret_cast<T*>(data_); }
    const T* operator->() const { return get(); }
    T* operator->() { return get(); }
  };

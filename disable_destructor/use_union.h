  // class for taking ownership and avoid call destructor
  template<typename T>
  class Indestructible {
    // use union trick to avoid call destructor
    union {
      std::remove_const_t<T> value_;
      struct {} dummy_;
    };
   public:
    template<typename... Args>
    Indestructible(Args &&... args) : value_(std::forward<Args>(args) ...) {}
    ~Indestructible() {}

    const T& value() const { return value_; }
    T& value() { return value_; }

    const T* get() const { return &value_; }
    T* get() { return &value_; }

    const T* operator->() const { return get(); }
    T* operator->() { return get(); }
  };

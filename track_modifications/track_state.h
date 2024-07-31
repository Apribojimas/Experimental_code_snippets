template<typename value_type>
class TrackState {
 public:
  TrackState() : modified_(false), value_{} {}
  explicit TrackState(value_type v) : modified_(false), value_(v) {}
  virtual ~TrackState() = default;

  value_type &operator*() noexcept { return value_; }
  operator value_type const &() const noexcept { return value_; }
  operator value_type &() noexcept { return value_; }
  operator value_type() const noexcept { return value_; }
  auto get() const noexcept -> value_type const & { return value_; }
  void set(const value_type &v) {
    if (std::memcmp(&v, &value_, sizeof(value_type)) != 0) {
      value_ = v;
      modified(true);
    }
  }

  auto operator=(const value_type &v) -> TrackState & {
    set(v);
    return *this;
  }

  void modified(bool x) {
      modified_ = x;
  }
  
  bool isModified() const {
      return modified_;
  }
 private:
  bool modified_;
  value_type value_;
};

  template<class... Args>
  std::string format(std::string message, Args&&... args) noexcept {
    try {
      size_t length = std::snprintf(nullptr, 0, message.c_str(), args...);
      std::string out(length + 1, '\0');
      std::snprintf(const_cast<char *>(out.data()), out.size(), message.c_str(), args...);
      return out;
    } catch (std::exception&) {
      // We cannot log here, so return an empty string
      return "";
    }
  }




template <typename ...Args>
std::string format(char const* fmt, Args&& ...args) {
    auto const len(std::snprintf(nullptr, 0, fmt, std::forward<Args>(args)...));
    std::string ret(len + 1, '\0');
    std::sprintf(ret.data(), fmt, std::forward<Args>(args)...);
    return ret;
}

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

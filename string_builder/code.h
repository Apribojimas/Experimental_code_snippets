  template<class... Args>
  [[nodiscard]] std::string concat(const Args&... args)
  {
    if constexpr (sizeof...(args) == 0) {
      return {};
    } else {
      std::ostringstream os;
      ((os << args),...);
      return os.str();
    }
  }

// concatenates with separator
std::string join(const std::vector<std::string>& a_vec, const char* separator) {
  return std::accumulate(a_vec.begin(), a_vec.end(), std::string{},
    [separator](const auto& str1, const auto& str2) {
      return str1.empty() ? str2 : str1 + separator + str2;
    });
}

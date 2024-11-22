# Links to various string builder (concatination)
* [FastStrcat](https://github.com/jm4R/fast_strcat) - Utilities to concatenate strings which reduce number of allocations


















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

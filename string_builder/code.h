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

// avoid re-allocations
std::string join(const std::vector<std::string>& svec, const std::string& sepa) {
	if (svec.empty())	return {};
	/* calculate the overall size beforehand, to avoid re-allocations. */
	size_t value_len =
		std::accumulate(svec.cbegin(), svec.cend(), 0,
				[](size_t size, const std::string& s) {
					return size + s.size();
				}) + (svec.size() - 1) * sepa.length();

	std::string value;
	value.reserve(value_len);

	std::accumulate(svec.cbegin(), svec.cend(), std::ref(value),
			[&](std::string& s1, const std::string& s2)->std::string& {
				if (s1.empty())
					s1 = s2;
				else {
					s1.append(sepa);
					s1.append(s2);
				}
				return s1;
			});

	return value;
}

// std::next trick
std::string join(const std::vector<std::string>& vec, const std::string& delim = ", ") {
  if (vec.empty()) {
    return std::string();
  }

  return std::accumulate(std::next(vec.begin()), vec.end(), vec[0],
                         [&delim](const std::string& a, const std::string& b) { return a + delim + b; });
}

template <class Container> void tokenizer(const std::string& str, Container* cont,
                                          const std::string& delims, bool skipEmpty) {
  std::size_t current, previous = 0;

  current = str.find_first_of(delims);
  while (current != std::string::npos) {
    std::string token = str.substr(previous, current - previous);
    if (!(skipEmpty == true && token.empty())) {
      cont->push_back(token);
    }
    previous = current + 1;
    current = str.find_first_of(delims, previous);
  }

  cont->push_back(str.substr(previous, current - previous));
}

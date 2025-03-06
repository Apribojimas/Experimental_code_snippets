std::string Elapsed::get_elapsed_time_as_string(int64_t elapsed_in_microseconds) {
  std::chrono::microseconds elapsed_us(elapsed_in_microseconds);
  std::ostringstream os;
  bool foundNonZero = false;

  os.fill('0');

  typedef std::chrono::duration<int, std::ratio<86400 * 365>> years;
  const auto y = std::chrono::duration_cast<years>(elapsed_us);
  if (y.count() || foundNonZero) {
    foundNonZero = true;
    os << y.count() << "y:";
    elapsed_us -= y;
  }

  typedef std::chrono::duration<int, std::ratio<86400>> days;
  const auto d = std::chrono::duration_cast<days>(elapsed_us);
  if (d.count() || foundNonZero) {
    foundNonZero = true;
    os << d.count() << "d:";
    elapsed_us -= d;
  }

  const auto h = std::chrono::duration_cast<std::chrono::hours>(elapsed_us);
  if (h.count() || foundNonZero) {
    foundNonZero = true;
    os << h.count() << "h:";
    elapsed_us -= h;
  }

  const auto m = std::chrono::duration_cast<std::chrono::minutes>(elapsed_us);
  if (m.count() || foundNonZero) {
    foundNonZero = true;
    os << m.count() << "m:";
    elapsed_us -= m;
  }

  const auto s = std::chrono::duration_cast<std::chrono::seconds>(elapsed_us);
  if (s.count() || foundNonZero) {
    foundNonZero = true;
    os << s.count() << "s:";
    elapsed_us -= s;
  }

  const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_us);
  if (ms.count() || foundNonZero) {
    foundNonZero = true;
    os << std::setw(3) << ms.count() << "ms:";
    elapsed_us -= ms;
  }

  const auto us = std::chrono::duration_cast<std::chrono::microseconds>(elapsed_us);
  os << std::setw(3) << us.count() << "us";

  return os.str();
}

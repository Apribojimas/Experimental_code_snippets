
 template <class _Rep, class _Period>
    datetime& operator-= (std::chrono::duration<_Rep, _Period> duration)
    {
        (*this) -= timespan(duration);
        return *this;
    }
    
    
      inline std::string duration_to_string(std::chrono::steady_clock::duration duration)
      {
         auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration).count();
         auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count() % 60;
         auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;
         return std::format("{}m {}s {}ms", minutes, seconds, milliseconds);
      }
    
    
std::string Utils::formatDuration(std::chrono::seconds diff)
{
    std::string text;
    const auto h = std::chrono::duration_cast<std::chrono::hours>(diff);
    auto hours_count = h.count();
    if(hours_count > 0)
    {
        if(hours_count < 10) text="0";
        text+= std::to_string(hours_count) + "h:";
    }
    diff = diff - h;
    const auto m = std::chrono::duration_cast<std::chrono::minutes>(diff);
    auto min_count = m.count();
    if(hours_count > 0 || min_count > 0)
    {
        if(min_count < 10) text+="0";
        text += std::to_string(min_count) + "m:";
    }
    diff = diff - m;
    auto sec_count = diff.count();
    if(sec_count < 10) text+="0";
    text += std::to_string(sec_count)+"s";
    return text;
}    

std::string SimulationClock::time_point_to_string(time_point time_point) {
  std::array<int, 5> date;
  auto time_from_epoch = time_point.time_since_epoch();
  auto year_from_epoch = std::chrono::duration_cast<years>(time_from_epoch);
  auto hour_from_epoch = std::chrono::duration_cast<hours>(time_from_epoch);
  date[0] = year_from_epoch.count() + epoch_[0];
  auto hour_of_year_int = static_cast<int>((hour_from_epoch - year_from_epoch).count());
  auto day_of_year = hour_of_year_int / 24;
  auto shifted_day_of_year = day_of_year + (day_of_year > 59 ? -59 : 306);
  date[3] = hour_of_year_int % 24 + epoch_[3];
  auto temp = (5 * shifted_day_of_year + 2) / 153;
  date[2] = shifted_day_of_year - (153 * temp + 2) / 5 + epoch_[2];
  date[1] = temp + (temp < 10 ? 2 : -10) + epoch_[1];
  date[4] = (time_from_epoch - hour_from_epoch).count() + epoch_[4];
  return std::to_string(date[0]) + "-" + std::to_string(date[1]) + "-" + std::to_string(date[2]) + "_"
      + std::to_string(date[3]) + ":" + std::to_string(date[4]);
} // credits to Howard Hinnant for the idea to this brilliant algorithm


/**
 * \brief Convert a time point to a string representation
 * \param[in] time The time point
 * \return A string representing the time point in hh:mm:ss.nanoseconds format
 */
std::string time_point_to_string(const time_point &time)
{
	uint64_t nsecs = std::chrono::duration_cast<std::chrono::nanoseconds>(time.time_since_epoch()).count();
	unsigned int secs = nsecs / 1000000000ULL;

	std::ostringstream ossTimestamp;
	ossTimestamp.fill('0');
	ossTimestamp << secs / (60 * 60) << ":"
		     << std::setw(2) << (secs / 60) % 60 << ":"
		     << std::setw(2) << secs % 60 << "."
		     << std::setw(9) << nsecs % 1000000000ULL;
	return ossTimestamp.str();
}

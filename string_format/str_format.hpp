// KT header-only library
// Requirements: C++17

#pragma once
#include <ostream>
#include <sstream>
#include <string_view>

namespace kt {
///
/// \brief Format token
///
constexpr std::string_view fmt_token = "{}";

///
/// \brief Obtain formatted string
/// \param fmt Interpolated string format (only exact fmt_token literal supported for direct argument replacement)
/// \param args Arguments
///
template <typename... Args>
std::string format_str(std::string_view fmt, Args const&... args);

// impl

inline std::ostream& format_str(std::ostream& out, std::string_view fmt) { return out << fmt; }

template <typename Arg, typename... Args>
std::ostream& format_str(std::ostream& out, std::string_view fmt, Arg const& arg, Args const&... args) {
	if (auto i = fmt.find(fmt_token); i != std::string::npos) { return format_str(out << fmt.substr(0, i) << arg, fmt.substr(i + fmt_token.size()), args...); }
	return format_str(out, fmt);
}

template <typename... Args>
std::string format_str(std::string_view fmt, Args const&... args) {
	std::stringstream str;
	format_str(str, fmt, args...);
	return str.str();
}
} // namespace kt

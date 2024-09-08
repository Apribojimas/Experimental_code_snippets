#include <type_traits>
#include <string>
#include <sstream>

namespace stdexp {
  namespace detail {
    // define traits for string formating
    //-----------------------------------------------------------------------------
    template <typename T, typename = void>
    struct is_to_string_able : std::false_type {};

    template <typename T>
    struct is_to_string_able<T, std::void_t<decltype(std::to_string(std::declval<T>()))>> : std::true_type {};

    //-----------------------------------------------------------------------------
    template<typename S, typename V>
    using streamability = decltype(std::declval<S&>() << std::declval<const V&>());

    template<typename S, typename V, class = void>
    struct is_stream_able : std::false_type {};

    template<typename S, typename V>
    struct is_stream_able<S, V, std::void_t<streamability<S, V>>> : std::true_type {};
    //-----------------------------------------------------------------------------

    // convert arguments to string
    template <typename T>
    [[nodiscard]] std::string to_string(T&& value) {
      if constexpr (std::is_same<std::decay_t<T>, std::string>::value) {
        return value;
      } else if constexpr (is_to_string_able<std::decay_t<T>>::value) {
        return std::to_string(std::forward<T>(value));
      } else if constexpr (is_stream_able<std::stringstream, std::decay_t<T>>::value) {
        std::stringstream ss;
        ss << value;
        return ss.str();
      } else {
        // Unsupported type, require custom implementation of "to_string"
        static_assert(false, "Unsupported type");
      }
    }

    // replace std::string with std::string_view once available
    [[nodiscard]] std::string format(const std::string& fmt, const std::initializer_list<std::string>& args);

  }  // namespace detail

  template <typename... Args>
  [[nodiscard]] std::string format(const std::string& fmt, Args&&... args) {
    return detail::format(fmt, { detail::to_string(std::forward<Args>(args))... });
  }
}  // namespace stdexp

#include <numeric>

namespace stdexp {
  namespace detail {
    // The size of the placeholder in the format function.
    constexpr std::size_t PLACEHOLDER_SIZE = 2;

    std::string format(const std::string& fmt, const std::initializer_list<std::string>& args) {
      std::size_t pos = 0;
      std::string result{ fmt };

      auto const size = fmt.size() +
        std::accumulate(args.begin(), args.end(), std::size_t{ 0u },
          [](std::size_t c, auto const& str) { return c + str.size(); });
      result.reserve(size);

      for (const auto& arg : args) {
        pos = result.find("{}", pos);
        if (std::string::npos == pos) {
          // "Not enough placeholders found"
          break;
        }

        result.replace(pos, PLACEHOLDER_SIZE, arg);
        pos += arg.size();
      }

      return result;
    }
  }  // namespace detail
}  // namespace stdexp


#include <iostream>
int main() {
  std::cout << "Hello World!\n" << std::endl;

  // just char * to std::string
  std::cout << stdexp::format("off") << ": " <<"off" << std::endl;

  // for conversion used std::to_string
  std::cout << stdexp::format("{}", int(-1)) << ": " <<"-1" << std::endl;
  std::cout << stdexp::format("{}", long(-2)) << ": " <<"-2" << std::endl;
  std::cout << stdexp::format("{}", long long(-3)) << ": " <<"-3" << std::endl;
  std::cout << stdexp::format("{}", unsigned(1)) << ": " <<"1" << std::endl;
  std::cout << stdexp::format("{}", unsigned long(2)) << ": " <<"2" << std::endl;
  std::cout << stdexp::format("{}", unsigned long long(3)) << ": " <<"3" << std::endl;
  std::cout << stdexp::format("{}", float(0)) << ": " <<"0" << std::endl;
  std::cout << stdexp::format("{}", double(-0)) << ": " <<"0" << std::endl;
  std::cout << stdexp::format("{}", float(1.9e3)) << ": " <<"1900" << std::endl;
  std::cout << stdexp::format("{}", double(-1.9e3)) << ": " <<"-1900" << std::endl;
  std::cout << stdexp::format("{}", float(10)) << ": " <<"10" << std::endl;
  std::cout << stdexp::format("{}", double(-10)) << ": " <<"-10" << std::endl;
  std::cout << stdexp::format("{}", float(23.0)) << ": " <<"23" << std::endl;
  std::cout << stdexp::format("{}", double(-23.0)) << ": " <<"-23" << std::endl;
  std::cout << stdexp::format("{}", float(3.369)) << ": " <<"3.369" << std::endl;
  std::cout << stdexp::format("{}", double(-3.369)) << ": " <<"-3.369" << std::endl;
  std::cout << stdexp::format("{}", long double(123456.654321)) << ": " <<"123456.654321" << std::endl;
  std::cout << stdexp::format("{}", bool(0)) << ": " <<"false" << std::endl;
  std::cout << stdexp::format("{}", bool(1)) << ": " <<"true" << std::endl;

  // stream or to_string formating
  std::cout << stdexp::format("{}", unsigned short(11)) << ": " <<"11" << std::endl;
  std::cout << stdexp::format("{}", unsigned int(12)) << ": " <<"12" << std::endl;
  std::cout << stdexp::format("{}", short(-11)) << ": " <<"-11" << std::endl;

  // stream formating
  void* ptr(nullptr);
  std::cout << stdexp::format("{}", ptr) << ": " <<"00000000" << std::endl;
}


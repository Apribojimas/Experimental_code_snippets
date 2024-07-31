#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>


#include <iostream>
#include <chrono>
#include <array>
#include <vector>
#include <cstring>
#include <algorithm>


class Duration 
{
public:
  Duration(std::string n) : 
    start_time(std::chrono::high_resolution_clock::now()), name(n)
  {
  }

  ~Duration()
  {
    auto now = std::chrono::high_resolution_clock::now();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time);
    std::cout << name << ": " << milliseconds.count() << std::endl;
  }
protected:
  std::string name;
  std::chrono::high_resolution_clock::time_point start_time;
};


//////////////////////
// utilities

template <class Container> 
void tokenizer(const std::string& str, Container* cont, const std::string& delims, bool skipEmpty) {
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

namespace messages {
  constexpr char kStringParameterDelimiter = '\t';
}
size_t SplitParameters(const std::string& parameter_string, std::string* parameter_array, const size_t parameter_array_size) {
  size_t pos_start = 0;
  size_t pos_end = parameter_string.find(messages::kStringParameterDelimiter);
  size_t found_strings = 0;
  while ((found_strings < parameter_array_size) && (pos_end != std::string::npos)) {
    auto part = parameter_string.substr(pos_start, pos_end - pos_start);
    parameter_array[found_strings].swap(part);
    found_strings++;
    pos_start = pos_end + 1;
    pos_end = parameter_string.find(messages::kStringParameterDelimiter, pos_start);
  }
  // IAR needs a lvalue to swap :-(
  auto part = parameter_string.substr(pos_start);
  parameter_array[found_strings].swap(part);
  found_strings++;
  return found_strings;
}

void tokenize(std::string str, std::vector<std::string>& token_v) {
  size_t start = str.find_first_not_of(messages::kStringParameterDelimiter), end = start;

  while (start != std::string::npos) {
    // Find next occurence of delimiter
    end = str.find(messages::kStringParameterDelimiter, start);
    // Push back the token found into vector
    token_v.push_back(str.substr(start, end - start));
    // Skip all occurences of the delimiter to find new start
    start = str.find_first_not_of(messages::kStringParameterDelimiter, end);
  }
}

namespace find
{
  void tokenize(std::string const& str, const char delim, std::vector<std::string>& out)
  {
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
      end = str.find(delim, start);
      out.push_back(str.substr(start, end - start));
    }
  }
}

namespace getline
{
  void tokenize(std::string const& str, const char delim, std::vector<std::string>& out)
  {
    // construct a stream from the string
    std::stringstream ss(str);

    std::string s;
    while (std::getline(ss, s, delim)) {
      out.push_back(s);
    }
  }
}

namespace nstrtok
{
  void tokenize(std::string const& str, const char* delim, std::vector<std::string>& out)
  {
    char* token = strtok(const_cast<char*>(str.c_str()), delim);
    while (token != nullptr)
    {
      out.push_back(std::string(token));
      token = strtok(nullptr, delim);
    }
  }

  void tokenize(std::string const& str, const char delim, std::vector<std::string>& out)
  {
    char* token = std::strtok(const_cast<char*>(str.c_str()), &delim);
    while (token != nullptr)
    {
      out.push_back(std::string(token));
      token = std::strtok(nullptr, &delim);
    }
  }

  template <class Container>
  void tokenize(std::string const& str, const char delim, Container& out)
  {
    char* token = std::strtok(const_cast<char*>(str.c_str()), &delim);
    for (int i = 0; token != nullptr; ++i)
    {
      out[i] = token;
      token = std::strtok(nullptr, &delim);
    }
  }

  void tokenize(std::string const& str, const char delim, std::string * parameter_array, const size_t parameter_array_size)
  {
    char* token = std::strtok(const_cast<char*>(str.c_str()), &delim);
    for (int i = 0; token != nullptr; ++i)
    {
      parameter_array[i] = token;
      token = std::strtok(nullptr, &delim);
    }
  }
}

namespace strfind
{
  void tokenize(const std::string& s, const char delim, std::vector<std::string>& out)
  {
    std::string::size_type beg = 0;
    for (auto end = 0; (end = s.find(delim, end)) != std::string::npos; ++end)
    {
      out.push_back(s.substr(beg, end - beg));
      beg = end + 1;
    }

    out.push_back(s.substr(beg));
  }
}

namespace custom_strtok
{
  char* custom_strtok(char* string, const char* control) {
    static char* TOKEN = nullptr;
    char* str = NULL;

    if (string == NULL) {
      str = TOKEN;
      if (*str == 0) return NULL;
    }
    else {
      str = string;
    }

    string = str;
    for (; *str; str++) {
      if (*str == *control) {
        *str++ = '\0';
        break;
      }
    }

    TOKEN = str;
    return string;
  }

  void tokenize(std::string const& str, const char delim, std::string* parameter_array, const size_t parameter_array_size)
  {
    char* token = custom_strtok(const_cast<char*>(str.c_str()), &delim);
    for (int i = 0; token != nullptr; ++i)
    {
      parameter_array[i] = token;
      token = custom_strtok(nullptr, &delim);
    }
  }
}


namespace tcbrindle
{
  template <class InputIt, class ForwardIt, class BinOp>
  void for_each_token(InputIt first, InputIt last, ForwardIt d_first, ForwardIt d_last, BinOp binary_op)
  {
    while (first != last) {
      const auto pos = std::find_first_of(first, last, d_first, d_last);
      binary_op(first, pos);
      if (pos == last) break;
      first = std::next(pos);
    }
  }

  void tokenize(std::string const& str, const char delim, std::string* parameter_array, const size_t parameter_array_size)
  {
    int i(0);
    constexpr char delims[] = "\t";
    for_each_token(std::cbegin(str), std::cend(str), std::cbegin(delims), std::cend(delims), 
      [&](auto first, auto second) {
        if (first != second) {
          parameter_array[i++] = std::string(first, second);
        }
      });
  }
}

namespace split
{
  std::vector<std::string> split(const std::string& str, const std::string& delim)
  {
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
      pos = str.find(delim, prev);
      if (pos == std::string::npos) pos = str.length();
      std::string token = str.substr(prev, pos - prev);
      if (!token.empty()) 
        tokens.push_back(token);
      prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
  }
}

namespace martin
{
  typedef void(*split_fn)(const char*, size_t, void*);
  void split(const char* str, char sep, split_fn fun, void* data)
  {
    unsigned int start = 0, stop;
    for (stop = 0; str[stop]; stop++) {
      if (str[stop] == sep) {
        fun(str + start, stop - start, data);
        start = stop + 1;
      }
    }
    fun(str + start, stop - start, data);
  }

  void tokenize(const char* str, const char delim, std::string* parameter_array, const size_t parameter_array_size)
  {
    unsigned int start = 0, stop, index = 0;
    for (stop = 0; str[stop] && index < parameter_array_size; ++stop) {
      if (str[stop] == delim) {
        parameter_array[index++].assign(str + start, stop - start);
        start = stop + 1;
      }
    }
    if (index < parameter_array_size) {
      parameter_array[index].assign(str + start, stop - start);
    }
  }
}


constexpr int TEST_COUNT = 10000;

int main()
{
  std::string test("0\tbattery\t32ta");
  //std::string test{ "0\t{\"name\": \"entry0\", \"string\": \"Sample\"}\ttest" };

  {
    Duration d("tokenizer");

    for (int i = 0; i < TEST_COUNT; ++i) {
      std::string request(test);
      std::vector<std::string> parameter;
      tokenizer(request, &parameter, "\t", false);
    }
  }

  {
    Duration d("SplitParameters");

    for (int i = 0; i < TEST_COUNT; ++i) {
      std::string request(test);
      enum { ID, ELEMENT, VALUE, COUNT };
      std::array<std::string, COUNT> parameter;
      SplitParameters(request, parameter.data(), parameter.size());
    }
  }

  {
    Duration d("tokenize");

    for (int i = 0; i < TEST_COUNT; ++i) {
      std::string request(test);
      std::vector<std::string> parameter;
      tokenize(request, parameter);
    }
  }

  {
    Duration d("find");

    for (int i = 0; i < TEST_COUNT; ++i) {
      std::string request(test);
      std::vector<std::string> parameter;
      find::tokenize(request, messages::kStringParameterDelimiter, parameter);
    }
  }

  {
    Duration d("getline");

    for (int i = 0; i < TEST_COUNT; ++i) {
      std::string request(test);
      std::vector<std::string> parameter;
      getline::tokenize(request, messages::kStringParameterDelimiter, parameter);
    }
  }

  {
    Duration d("strtok");

    for (int i = 0; i < TEST_COUNT; ++i) {
      std::string request(test);
      std::vector<std::string> parameter;
      nstrtok::tokenize(request, &messages::kStringParameterDelimiter, parameter);
    }
  }

  {
    Duration d("std::strtok");

    for (int i = 0; i < TEST_COUNT; ++i) {
      std::string request(test);
      std::vector<std::string> parameter;
      nstrtok::tokenize(request, messages::kStringParameterDelimiter, parameter);
    }
  }

  {
    Duration d("strtok - array");

    for (int i = 0; i < TEST_COUNT; ++i) {
      std::string request(test);
      enum { ID, ELEMENT, VALUE, COUNT };
      std::array<std::string, COUNT> parameter;
      nstrtok::tokenize(request, messages::kStringParameterDelimiter, parameter);
    }
  }

  {
    Duration d("strtok - string[]");

    for (int i = 0; i < TEST_COUNT; ++i) {
      std::string request(test);
      enum { ID, ELEMENT, VALUE, COUNT };
      std::array<std::string, COUNT> parameter;
      nstrtok::tokenize(request, messages::kStringParameterDelimiter, parameter.data(), parameter.size());
    }
  }

  {
    Duration d("strfind");

    for (int i = 0; i < TEST_COUNT; ++i) {
      std::string request(test);
      std::vector<std::string> parameter;
      strfind::tokenize(request, messages::kStringParameterDelimiter, parameter);
    }
  }

  {
    Duration d("custom_strtok");

    for (int i = 0; i < TEST_COUNT; ++i) {
      std::string request(test);
      enum { ID, ELEMENT, VALUE, COUNT };
      std::array<std::string, COUNT> parameter;
      custom_strtok::tokenize(request, messages::kStringParameterDelimiter, parameter.data(), parameter.size());
    }
  }

  {
    Duration d("tcbrindle");

    for (int i = 0; i < TEST_COUNT; ++i) {
      std::string request(test);
      enum { ID, ELEMENT, VALUE, COUNT };
      std::array<std::string, COUNT> parameter;
      tcbrindle::tokenize(request, messages::kStringParameterDelimiter, parameter.data(), parameter.size());
    }
  }

  {
    Duration d("split");
    
    for (int i = 0; i < TEST_COUNT; ++i) {
      std::string request(test);
      auto v = split::split(request, "\t");
    }
  }

  {
    Duration d("martin");

    for (int i = 0; i < TEST_COUNT; ++i) {
      std::string request(test);
      enum { ID, ELEMENT, VALUE, COUNT };
      std::array<std::string, COUNT> parameter;
      martin::tokenize(request.c_str(), messages::kStringParameterDelimiter, parameter.data(), parameter.size());
    }
  }

}


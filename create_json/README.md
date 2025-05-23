Exist need for simple JSON creation something like:
auto j = json(
  {
    {"key1": fn1()},
    {"array1": fn2()}
  }

Must be oriented to embedded development. Small ROM size overhead!

Almost [NEAT JSON BUILDER for RAPIDJSON](https://github.com/crea7or/rapid-builder) - Neat JSON builder for rapidjson library that is based on std::initializer_list. Don't support temporary variables!!!

# Source of ideas
* [JSON Utilities](https://github.com/TimSevereijns/JSON-Utilities) - A series of functions that wrap the rapidjson library to enable easier serialization and deserialization. !!!
* [BlockTheSpot](https://github.com/mrpond/BlockTheSpot/blob/master/src/BasicUtils/Json.h) - Json value with initializer_list
* [A simple wrapper around](https://github.com/DenSinH/json/blob/master/main.cpp) - A simple JSON loader / dumper
* [template_remover.hpp](https://github.com/fcpp/androidDemoApp/blob/master/fcpp-android/lib/common/template_remover.hpp) - to_json
* [variant.hpp](https://github.com/malasiot/cvx/blob/master/include/cvx/misc/variant.hpp#L305) - variant with tojson
* [function.cpp](https://github.com/AidenSwayne/QuickFunctions/blob/main/c_plus_plus/json_encoder/function.cpp) - function json encoder
* [zypp](https://github.com/openSUSE/libzypp/blob/master/zypp/base/Json.h) - json encoder. Support many types for tojson functionality
* [json11](https://github.com/subhagho/ReactFS/blob/master/common/src/json11/json.h) - JsonValue
* [json11.h](https://github.com/jdeng/json11/blob/master/json11.h) - ValueHolder
* [jsonpp](https://github.com/Rapptz/jsonpp) - json::object
* [fwk](https://github.com/SanZhangO-O/fwk/blob/main/rpc/json.hpp) - encode
* [simple encoder](https://github.com/photonmaster/named_tuples/blob/master/json_encoder.h) - json simple encoder
* [sr](https://github.com/MinecraftSRDEV/SRLauncher/blob/main/src/headers/Classes/json_encoder.hpp) - jsonvalue class
* [centest](https://github.com/leonidk/centest/blob/learning/src/json.h) - typed json value
* [capturinha](https://github.com/kebby/Capturinha/blob/main/json.h) - json builder and write visitor
* [Value.h](https://github.com/zoltan-hadur/Cpp-OpenGL-GUI/blob/master/Json4CPP/Value.h) - std::variant<std::nullptr_t, std::wstring, bool, double, int64_t, JsonObject, JsonArray, Args...>;
* [cfdcore_json_writer.h](https://github.com/p2pderivatives/cfd-core/blob/master/include/cfdcore/cfdcore_json_writer.h) - Json builder
* [json20](https://github.com/ondra-novak/json20) - JSON support for C++20 / constexpr
* [cpp-json](https://github.com/eteran/cpp-json/blob/master/lib/include/cpp-json/json_encode.h) - A fast & modern C++17 JSON library
* [cpp-json](https://github.com/eteran/cpp-json/blob/master/lib/include/cpp-json/json_value.h) - A fast & modern C++17 JSON library
* [stdxx-json](https://github.com/JeneLitsch/stdxx/tree/master/stdxx/json) - Another implementation
* [psibase](https://github.com/gofractally/psibase/blob/main/libraries/psio/include/psio/to_json.hpp) - various to_json implementations
* [Safe JSON](https://github.com/ZY4N/Safe-JSON) - A JSON library made for config files.
* [basic_value.hpp](https://github.com/taocpp/json/blob/main/include/tao/json/basic_value.hpp) - variant json value
* [Tiny-JSON](https://github.com/Syan-Lin/Tiny-JSON/blob/main/examples.cpp) - have example
* [basic_json](https://github.com/KaixoCode/basic_json) - Simple JSON library with parser that also supports HJSON.

# Serialization
* [rapidjson_utils.h](https://github.com/valhalla/valhalla/blob/master/valhalla/baldr/rapidjson_utils.h) - This takes whatever data you want and serializes it directly to a json string rather than keeping it organized as json in an in memory object.

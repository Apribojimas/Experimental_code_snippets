Exist need for simple JSON creation something like:
auto j = json(
  {
    {"key1": fn1()},
    {"array1": fn2()}
  }

Must be oriented to embedded development. Small ROM size overhead!

# Source of ideas
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

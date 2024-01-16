#pragma once

#include "json.hpp"
using namespace nlohmann;

string wstring_to_string(const wstring& wstr);
wstring string_to_wstring(const string& str);
json vector_wstring_to_json(const vector<wstring>& vec);
vector<wstring> json_to_vector_wstring(const json& j);
json vec2_to_json(const Vec2& v);
Vec2 json_to_vec2(const json& j);
json vectorVec2_to_json(const vector<Vec2>& vec);
vector<Vec2> json_to_vectorVec2(const json& j);
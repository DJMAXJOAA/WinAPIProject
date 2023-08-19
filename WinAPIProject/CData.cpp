#include "pch.h"
#include "CData.h"

#include <codecvt>
#include <locale>

CData::CData(int _iID)
	: m_iID(_iID)
{
}

CData::~CData()
{
}

string wstring_to_string(const wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(wstr);
}

wstring string_to_wstring(const string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(str);
}

json vector_wstring_to_json(const vector<wstring>& vec)
{
	json j;
	for (const auto& wstr : vec) {
		j.push_back(wstring_to_string(wstr));
	}
	return j;
}

vector<wstring> json_to_vector_wstring(const json& j)
{
	std::vector<std::wstring> vec;
	for (const auto& str : j) {
		vec.push_back(string_to_wstring(str.get<std::string>()));
	}
	return vec;
}

json vec2_to_json(const Vec2& v)
{
	return json{ v.x, v.y };
}

Vec2 json_to_vec2(const json& j)
{
	Vec2 v;
	if (j.is_array() && j.size() == 2) {
		v.x = j[0].get<float>();
		v.y = j[1].get<float>();
	}
	return v;
}

#pragma once

#include "json.hpp"
using namespace nlohmann;
#include<fstream>
using std::locale;
using std::ifstream;
using std::ofstream;

string wstring_to_string(const wstring& wstr);
wstring string_to_wstring(const string& str);
json vector_wstring_to_json(const vector<wstring>& vec);
vector<wstring> json_to_vector_wstring(const json& j);
json vec2_to_json(const Vec2& v);
Vec2 json_to_vec2(const json& j);

class CData
{
private:
	int		m_iID;		// 데이터 id 정보

public:
	CData(int _iID);
	virtual ~CData();

public:
	int GetKey() { return m_iID; }

public:
	virtual void LoadData(const wstring& _filename);	// json을 불러와서 데이터 파싱
	virtual CData* FindData() { return this; }
	virtual void ParseData(const json& item) = 0;
};

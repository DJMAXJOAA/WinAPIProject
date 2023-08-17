#pragma once
class CObject;

void CreateObj(CObject* _pObj, GROUP_TYPE _eGroup);
void DeleteObj(CObject* _pObj);
void ChangeScene(SCENE_TYPE _eNext);

// Clear Vector Container
template<typename T>
void SafeDeleteVec(vector<T>& _vec)
{
	for (size_t i = 0; i < _vec.size(); i++)
	{
		if (_vec[i] != nullptr)
			delete _vec[i];
	}
	_vec.clear();
}

// Clear Map Container
template<typename T1, typename T2>
void SafeDeleteMap(map<T1, T2>& _map)
{
	// 템플릿 안에서 클래스 사용하려면, 클래스 앞에 typename 붙여줘야 한다
	typename map<T1, T2>::iterator iter = _map.begin();

	for (; iter != _map.end(); ++iter)
	{
		if (iter->second != nullptr)
		{
			delete iter->second;
		}
	}
	_map.clear();
}
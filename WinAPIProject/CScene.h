#pragma once

#include "CMonsterFactory.h"

class CObject;

class CScene
{
private:
	vector<CObject*>	m_arrObj[(UINT)GROUP_TYPE::END];
	wstring				m_strName;

	UINT				m_iTileX;	// 타일 가로 갯수
	UINT				m_iTileY;	// 타일 세로 갯수

public:
	CScene();
	virtual ~CScene();

public:
	void AddObject(CObject* pObj, GROUP_TYPE eType) { m_arrObj[(UINT)eType].push_back(pObj); }
	void DeleteGroup(GROUP_TYPE _eTarget);
	void DeleteAll();

	void CreateTile(UINT _iXCount, UINT _iYCount);
	void LoadTile(const wstring& _strRelativePath);

	vector<CObject*>& GetUIGroup() { return m_arrObj[(UINT)GROUP_TYPE::UI]; }

public:
	const wstring& GetName() { return m_strName; }
	const vector<CObject*>& GetGroupObject(GROUP_TYPE _eType) { return m_arrObj[(UINT)_eType]; } // 레퍼런스로 주되, 변경은 불가능하게 const

	UINT GetTileX() { return m_iTileX; }
	UINT GetTileY() { return m_iTileY; }

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }

	// 순수 가상함수 -> 직접 객체 생성이 불가능하다 (추상화 클래스)
	virtual void Enter() = 0; // Scene의 진입
	virtual void Exit() = 0; // Scene의 탈출

public:
	virtual void Update();
	virtual void FinalUpdate(); // 마무리 작업(모두 업데이트 끝나고, 확정 후 render)
	virtual void Render(HDC hdc);
};


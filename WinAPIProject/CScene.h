#pragma once

#include "CMonsterFactory.h"

class CObject;

class CScene
{
private:
	vector<CObject*>	m_arrObj[(UINT)GROUP_TYPE::END];
	wstring				m_strName;

	UINT				m_iTileX;	// Ÿ�� ���� ����
	UINT				m_iTileY;	// Ÿ�� ���� ����

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
	const vector<CObject*>& GetGroupObject(GROUP_TYPE _eType) { return m_arrObj[(UINT)_eType]; } // ���۷����� �ֵ�, ������ �Ұ����ϰ� const

	UINT GetTileX() { return m_iTileX; }
	UINT GetTileY() { return m_iTileY; }

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }

	// ���� �����Լ� -> ���� ��ü ������ �Ұ����ϴ� (�߻�ȭ Ŭ����)
	virtual void Enter() = 0; // Scene�� ����
	virtual void Exit() = 0; // Scene�� Ż��

public:
	virtual void Update();
	virtual void FinalUpdate(); // ������ �۾�(��� ������Ʈ ������, Ȯ�� �� render)
	virtual void Render(HDC hdc);
};


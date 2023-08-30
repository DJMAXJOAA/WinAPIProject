#pragma once

class CObject;

class CScene
{
private:
	vector<CObject*>	m_arrObj[(UINT)GROUP_TYPE::END];
	wstring				m_strName;

public:
	CScene();
	virtual ~CScene();

public:
	void AddObject(CObject* pObj, GROUP_TYPE eType) { m_arrObj[(UINT)eType].push_back(pObj); }
	void DeleteGroup(GROUP_TYPE _eTarget);
	void DeleteAll();

	vector<CObject*>& GetUIGroup() { return m_arrObj[(UINT)GROUP_TYPE::UI]; }

public:
	wstring& GetName() { return m_strName; }
	vector<CObject*>& GetGroupObject(GROUP_TYPE _eType) { return m_arrObj[(UINT)_eType]; } // ���۷����� �ֵ�, ������ �Ұ����ϰ� const

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }

	// ���� �����Լ� -> ���� ��ü ������ �Ұ����ϴ� (�߻�ȭ Ŭ����)
	virtual void Enter() = 0; // Scene�� ����
	virtual void Exit() = 0; // Scene�� Ż��

	virtual void CameraEvent() {};

public:
	virtual void Update();
	virtual void FinalUpdate(); // ������ �۾�(��� ������Ʈ ������, Ȯ�� �� render)
	virtual void Render(HDC hdc);
};


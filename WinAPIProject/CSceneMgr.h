#pragma once

class CScene;

class CSceneMgr
{
	SINGLE(CSceneMgr);

private:
	CScene* m_arrScene[(UINT)SCENE_TYPE::END];
	CScene* m_pCurScene;

private:
	CSceneMgr();
	~CSceneMgr();

public:
	CScene* GetCurScene() { return m_pCurScene; }
	
public:
	void Init();
	void Update();
	void Render(HDC hdc);

private:
	// ���� �Լ��� ȣ�� -> �̺�Ʈ �Ŵ����� ��� -> �̺�Ʈ ó�� -> �״��� �� ����
	void ChangeScene(SCENE_TYPE _eNext);

	friend class CEventMgr;
};


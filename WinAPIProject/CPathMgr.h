#pragma once
class CPathMgr
{
	SINGLE(CPathMgr);
private:
	wchar_t		m_szContentPath[256];	// 윈도우 최대 글자 255자 제한
	wchar_t		m_szRelativePath[256];

private:
	CPathMgr();
	~CPathMgr();

public:
	void Init();
	const wchar_t* GetContentPath() { return m_szContentPath; }
	wstring GetRelativePath(const wchar_t* _filepath);
};


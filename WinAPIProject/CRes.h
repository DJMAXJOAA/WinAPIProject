#pragma once
class CRes
{
private:
	wstring	m_strKey;			// 리소스 키
	wstring	m_strRelativePath;	// 리소스 상대 경로 (로딩 되었을때 경로 저장)

public:
	CRes();
	virtual ~CRes();	// 오브젝트처럼 주의해야댐

public:
	void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
	void SetRelativePath(const wstring& _strPath) { m_strRelativePath = _strPath; }

	const wstring& GetKey() { return m_strKey; }
	const wstring& GetRleativePath() { return m_strRelativePath; }
};
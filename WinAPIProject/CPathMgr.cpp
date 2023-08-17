#include "pch.h"
#include "CPathMgr.h"

#include "CCore.h"

CPathMgr::CPathMgr()
	: m_szContentPath{}
{
}

CPathMgr::~CPathMgr()
{
}

void CPathMgr::Init()
{
	GetCurrentDirectory(255, m_szContentPath); // 현재 경로를 가져온다

	int iLen = (int)wcslen(m_szContentPath);
	for (int i = iLen - 1; i >= 0; i--)
	{
		if ('\\' == m_szContentPath[i])
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}
	wcscat_s(m_szContentPath, 255, L"\\bin\\content\\"); // 문자열 이어붙여주기
}

wstring CPathMgr::GetRelativePath(const wchar_t* _filepath)
{
	wstring strFilePath = _filepath;

	size_t iAbsLen = wcslen(m_szContentPath);
	size_t iFullLen = strFilePath.length();

	wstring strRealativePath = strFilePath.substr(iAbsLen, iFullLen - iAbsLen);

	return strRealativePath;
}

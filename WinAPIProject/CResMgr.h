#pragma once

class CRes;
class CTexture;
class CGdiPlus;

class CResMgr
{
	SINGLE(CResMgr);

private:
	map<wstring, CRes*>			m_mapTex;
	map<wstring, CRes*>			m_mapGdiPlus;

private:
	CResMgr();
	~CResMgr();

public:
	CTexture* LoadTexture(const wstring& _strKey, const wstring& _strRelativePath);
	CTexture* CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight);
	CTexture* FindTexture(const wstring& _strKey);

public:
	CGdiPlus* LoadGdiPlus(const wstring& _strKey, const wstring& _strRelativePath);
	CGdiPlus* FindGdiPlus(const wstring& _strKey);
};


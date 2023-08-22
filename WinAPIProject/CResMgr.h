#pragma once

class CRes;
class CTexture;
class CGdiPlus;

class CResMgr
{
	SINGLE(CResMgr);

private:
	map<wstring, CRes*>			m_mapTex;
	map<wstring, CRes*>			m_mapGDIplus;

private:
	CResMgr();
	~CResMgr();

public:
	void PrintTextureList();

public:
	CTexture* LoadTexture(const wstring& _strKey, const wstring& _strRelativePath);
	CTexture* CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight);
	CTexture* FindTexture(const wstring& _strKey);

public:
	CGdiPlus* LoadGDIplus(const wstring& _strKey, const wstring& _strRelativePath);
	CGdiPlus* FindGDIplus(const wstring& _strKey);
};


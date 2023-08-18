#pragma once

class CRes;
class CTexture;
class CAnimation;

class CResMgr
{
	SINGLE(CResMgr);

private:
	map<wstring, CRes*>			m_mapTex;
	map<wstring, CAnimation*>	m_mapAnim;

private:
	CResMgr();
	~CResMgr();

public:
	void PrintTextureList();

public:
	CTexture* LoadTexture(const wstring& _strKey, const wstring& _strRelativePath);
	CTexture* CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight);
	CTexture* FindTexture(const wstring& _strKey);
	CAnimation* CreateAnimation(const wstring& _strName, CTexture* _pTex, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount);
	CAnimation* FindAnimation(const wstring& _strName);
};


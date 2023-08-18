#include "pch.h"
#include "CResMgr.h"

#include "CPathMgr.h"

#include "CTexture.h"
#include "CAnimation.h"

CResMgr::CResMgr()
{
}

CResMgr::~CResMgr()
{
    // 맵의 모든 텍스쳐 할당을 해제시켜준다
    SafeDeleteMap(m_mapTex);
    SafeDeleteMap(m_mapAnim);
}

void CResMgr::PrintTextureList()
{
    // 콘솔 디버그용
    map<wstring, CRes*> m_mapTex;
    for (auto iter = m_mapTex.begin(); iter != m_mapTex.end(); iter++)
    {
        cout << iter->first.c_str() << ",";
    }
    cout << endl;
}

CTexture* CResMgr::LoadTexture(const wstring& _strKey, const wstring& _strRelativePath)
{
    // 키값과, 상대경로를 인자로 넣는다
    // 중복된 키 값이 들어오면, 새로 만들어서 등록하는게 아니라, 먼저 검색 후 있으면 원래 있던거 반환
    CTexture* pTex = FindTexture(_strKey);
    if (pTex != nullptr)
    {
        return pTex;
    }

    // 키값을 검색 해봤는데 비트맵 정보가 맵에 없으면, 그땐 새로 불러와서 만들고 등록
    wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
    strFilePath += _strRelativePath;

    pTex = new CTexture;
    pTex->Load(strFilePath);
    pTex->SetRelativePath(_strRelativePath); // 정보를 함께 넣어준다

    m_mapTex.insert(make_pair(_strKey, pTex));

    return pTex;
}

CTexture* CResMgr::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight)
{
    // 키값과, 상대경로를 인자로 넣는다
    // 중복된 키 값이 들어오면, 새로 만들어서 등록하는게 아니라, 먼저 검색 후 있으면 원래 있던거 반환
    CTexture* pTex = FindTexture(_strKey);
    if (pTex != nullptr)
    {
        return pTex;
    }

    pTex = new CTexture;
    pTex->Create(_iWidth, _iHeight);
    pTex->SetKey(_strKey);

    m_mapTex.insert(make_pair(_strKey, pTex));

    return pTex;
}

CTexture* CResMgr::FindTexture(const wstring& _strKey)
{
    map<wstring, CRes*>::iterator iter = m_mapTex.find(_strKey); // 이터레이터 값으로 반환
    if (iter == m_mapTex.end())
    {
        return nullptr; // 새로 만들어야 함
    }

    return (CTexture*)iter->second; // 발견값의 두번째 값, 즉 이미지 정보가 들어있는 cTexture 객체를 반환
}

CAnimation* CResMgr::CreateAnimation(const wstring& _strName, CTexture* _pTex, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
    CAnimation* pAnim = FindAnimation(_strName);
    if (pAnim != nullptr)
    {
        return pAnim;
    }

    // 존재 안하면 애니메이션 새로 하나 만듬
    pAnim = new CAnimation;
    pAnim->SetName(_strName);
    pAnim->Create(_pTex, _vLeftTop, _vSliceSize, _vStep, _fDuration, _iFrameCount);
    m_mapAnim.insert(make_pair(_strName, pAnim));

    return pAnim;
}

CAnimation* CResMgr::FindAnimation(const wstring& _strName)
{
    map<wstring, CAnimation*>::iterator iter = m_mapAnim.find(_strName);
    if (iter == m_mapAnim.end())
    {
        return nullptr;
    }

    return iter->second;
}
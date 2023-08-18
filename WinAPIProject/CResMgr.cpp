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
    // ���� ��� �ؽ��� �Ҵ��� ���������ش�
    SafeDeleteMap(m_mapTex);
    SafeDeleteMap(m_mapAnim);
}

void CResMgr::PrintTextureList()
{
    // �ܼ� ����׿�
    map<wstring, CRes*> m_mapTex;
    for (auto iter = m_mapTex.begin(); iter != m_mapTex.end(); iter++)
    {
        cout << iter->first.c_str() << ",";
    }
    cout << endl;
}

CTexture* CResMgr::LoadTexture(const wstring& _strKey, const wstring& _strRelativePath)
{
    // Ű����, ����θ� ���ڷ� �ִ´�
    // �ߺ��� Ű ���� ������, ���� ���� ����ϴ°� �ƴ϶�, ���� �˻� �� ������ ���� �ִ��� ��ȯ
    CTexture* pTex = FindTexture(_strKey);
    if (pTex != nullptr)
    {
        return pTex;
    }

    // Ű���� �˻� �غôµ� ��Ʈ�� ������ �ʿ� ������, �׶� ���� �ҷ��ͼ� ����� ���
    wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
    strFilePath += _strRelativePath;

    pTex = new CTexture;
    pTex->Load(strFilePath);
    pTex->SetRelativePath(_strRelativePath); // ������ �Բ� �־��ش�

    m_mapTex.insert(make_pair(_strKey, pTex));

    return pTex;
}

CTexture* CResMgr::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight)
{
    // Ű����, ����θ� ���ڷ� �ִ´�
    // �ߺ��� Ű ���� ������, ���� ���� ����ϴ°� �ƴ϶�, ���� �˻� �� ������ ���� �ִ��� ��ȯ
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
    map<wstring, CRes*>::iterator iter = m_mapTex.find(_strKey); // ���ͷ����� ������ ��ȯ
    if (iter == m_mapTex.end())
    {
        return nullptr; // ���� ������ ��
    }

    return (CTexture*)iter->second; // �߰߰��� �ι�° ��, �� �̹��� ������ ����ִ� cTexture ��ü�� ��ȯ
}

CAnimation* CResMgr::CreateAnimation(const wstring& _strName, CTexture* _pTex, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
    CAnimation* pAnim = FindAnimation(_strName);
    if (pAnim != nullptr)
    {
        return pAnim;
    }

    // ���� ���ϸ� �ִϸ��̼� ���� �ϳ� ����
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
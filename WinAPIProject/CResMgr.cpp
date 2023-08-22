#include "pch.h"
#include "CResMgr.h"

#include "CPathMgr.h"

#include "CTexture.h"
#include "CGdiPlus.h"

CResMgr::CResMgr()
{
}

CResMgr::~CResMgr()
{
    // ���� ��� �ؽ��� �Ҵ��� ���������ش�
    SafeDeleteMap(m_mapTex);
    SafeDeleteMap(m_mapGdiPlus);
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

    // �� �ؽ��� �����
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

CGdiPlus* CResMgr::LoadGdiPlus(const wstring& _strKey, const wstring& _strRelativePath)
{
    CGdiPlus* pTex = FindGdiPlus(_strKey);
    if (pTex != nullptr)
    {
        return pTex;
    }

    wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
    strFilePath += _strRelativePath;

    pTex = new CGdiPlus;
    pTex->Load(strFilePath);
    pTex->SetRelativePath(_strRelativePath);

    m_mapGdiPlus.insert(make_pair(_strKey, pTex));

    return pTex;
}

CGdiPlus* CResMgr::FindGdiPlus(const wstring& _strKey)
{
    map<wstring, CRes*>::iterator iter = m_mapGdiPlus.find(_strKey);
    if (iter == m_mapGdiPlus.end())
    {
        return nullptr;
    }

    return (CGdiPlus*)iter->second;
}

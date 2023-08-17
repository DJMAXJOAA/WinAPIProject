#include "pch.h"
#include "CTexture.h"

#include "CCore.h"

CTexture::CTexture()
	: m_hBit(0)
	, m_hdc(0)
	, m_bitInfo{}
{
}

CTexture::~CTexture()
{
	DeleteDC(m_hdc);
	DeleteObject(m_hBit);
}

void CTexture::Load(const wstring& _strFilePath)
{
	// ���� ��� -> �Ӽ�, �Ϲ� ���������� ��� ���丮�� $(SolutionDir)Output\bin, bin_debug�� �������־�� �Ѵ�
	// ���ҽ� ������ bin ���� ���� content ���� �ȿ� ����ȴ� (���� ���� �ȿ� ���ϵ��� �Բ� ����ְ� �ϱ� ���ؼ�)
	m_hBit = (HBITMAP)LoadImage(nullptr, _strFilePath.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	assert(m_hBit); // ���� ����

	// ��Ʈ�ʰ� ������ DC
	m_hdc = CreateCompatibleDC(CCore::GetInstance()->GetMainDC());

	// ��Ʈ�ʰ� dc ����
	HBITMAP hPrevBit = (HBITMAP)SelectObject(m_hdc, m_hBit);
	DeleteObject(hPrevBit);

	// ���� ���� ���� ����ִ� ��Ʈ�� ����
	BITMAP tInfo = {};
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}

void CTexture::Create(UINT _iWidth, UINT _iHeight)
{
	HDC mainDC = CCore::GetInstance()->GetMainDC();

	m_hBit = CreateCompatibleBitmap(mainDC, _iWidth, _iHeight);
	m_hdc = CreateCompatibleDC(mainDC); // hdc�� ������ dc ��������

	HBITMAP hOldBit = (HBITMAP)SelectObject(m_hdc, m_hBit);
	DeleteObject(hOldBit);

	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}


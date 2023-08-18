#pragma once
#include "CData.h"

class AnimationData
	: public CData
{
public:
	wstring				m_strTextureName;	// ������ �ִϸ��̼� �̸�
	wstring				m_strTexturePath;	// ������ ���� ���� ���
	
	int					m_iFrame;			// ���� ����?
	Vec2				m_vLeftTop;			// ���� ���� ��ġ
	Vec2				m_vSliceSize;		// ������ �ڸ�����
	Vec2				m_vOffset;			// ������ ���� ����

public:
	AnimationData(int _key);
	~AnimationData();

public:


public:
	virtual void LoadData() override;
};


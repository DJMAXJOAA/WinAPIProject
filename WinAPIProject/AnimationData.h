#pragma once
#include "CData.h"

struct AnimationInfo
{
	int					iFrame;			// ���� ����?
	Vec2				vLeftTop;		// ���� ���� ��ġ
	Vec2				vSliceSize;		// ������ �ڸ�����
	Vec2				vOffset;		// ������ ���� ����
};

// json�� ����ȭ, ������ȭ
void to_json(json& j, const AnimationInfo& p);
void from_json(const json& j, AnimationInfo& p);

class AnimationData
	: public CData
{
public:
	wstring				m_strTextureName;	// ������ �ִϸ��̼� �̸�
	wstring				m_strTexturePath;	// ������ ���� ���� ���
	
	AnimationInfo		m_AniInfo;			// ���� ����

public:
	AnimationData(int _key);
	~AnimationData();

public:


public:
	virtual void LoadData() override;
};


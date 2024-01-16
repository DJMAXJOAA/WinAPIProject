#pragma once
#include "CData.h"

struct AnimationInfo
{
	Vec2				vLeftTop;		// �ؽ��� ���� ��ġ(Left Top)
	Vec2				vSliceSize;		// ���ۺ��� ������ �ڸ�����(Right Bottom)
	Vec2				vStep;			// �ִϸ��̼� ���� �����Ӱ��� ����
	Vec2				vOffset;		// ������ ���� ����
	float				fDuration;		// �ִϸ��̼� ���ӽð�
	int					iFrame;			// �ִϸ��̼� ��������?
	int					iEventFrame;		// �̺�Ʈ ������ (0�̸� �ִϸ��̼� �̺�Ʈ ����)
};

// json�� ����ȭ, ������ȭ
void to_json(json& j, const AnimationInfo& a);
void from_json(const json& j, AnimationInfo& a);

class AnimationData
	: public CData
{
public:
	wstring				m_strAnimationName;	// ������ �ִϸ��̼� �̸�
	wstring				m_strTexturePath;	// ������ ���� ���� ���
	wstring				m_strAnimationType;	// �ִϸ��̼� Ÿ��
	AnimationInfo		m_AniInfo;			// �ִϸ��̼� ����

public:
	AnimationData(int _key);
	~AnimationData();

public:
	virtual void SaveData();
	virtual void ParseData(const json& item) override;
};


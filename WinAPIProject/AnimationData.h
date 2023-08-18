#pragma once
#include "CData.h"

class AnimationData
	: public CData
{
public:
	wstring				m_strTextureName;	// 생성된 애니메이션 이름
	wstring				m_strTexturePath;	// 가져올 폴더 파일 경로
	
	int					m_iFrame;			// 사진 몇장?
	Vec2				m_vLeftTop;			// 사진 시작 위치
	Vec2				m_vSliceSize;		// 어디까지 자를건지
	Vec2				m_vOffset;			// 오프셋 보정 정보

public:
	AnimationData(int _key);
	~AnimationData();

public:


public:
	virtual void LoadData() override;
};


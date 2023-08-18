#pragma once
#include "CData.h"

struct AnimationInfo
{
	int					iFrame;			// 사진 몇장?
	Vec2				vLeftTop;		// 사진 시작 위치
	Vec2				vSliceSize;		// 어디까지 자를건지
	Vec2				vOffset;		// 오프셋 보정 정보
};

// json의 직렬화, 역직렬화
void to_json(json& j, const AnimationInfo& p);
void from_json(const json& j, AnimationInfo& p);

class AnimationData
	: public CData
{
public:
	wstring				m_strTextureName;	// 생성된 애니메이션 이름
	wstring				m_strTexturePath;	// 가져올 폴더 파일 경로
	
	AnimationInfo		m_AniInfo;			// 사진 정보

public:
	AnimationData(int _key);
	~AnimationData();

public:


public:
	virtual void LoadData() override;
};


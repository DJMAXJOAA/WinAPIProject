#pragma once
#include "CData.h"

struct AnimationInfo
{
	Vec2				vLeftTop;		// 텍스쳐 시작 위치(Left Top)
	Vec2				vSliceSize;		// 시작부터 어디까지 자를건지(Right Bottom)
	Vec2				vStep;			// 애니메이션 다음 프레임간의 간격
	Vec2				vOffset;		// 오프셋 보정 정보
	float				fDuration;		// 애니메이션 지속시간
	int					iFrame;			// 애니메이션 몇프레임?
	int					iEventFrame;		// 이벤트 프레임 (0이면 애니메이션 이벤트 없음)
};

// json의 직렬화, 역직렬화
void to_json(json& j, const AnimationInfo& a);
void from_json(const json& j, AnimationInfo& a);

class AnimationData
	: public CData
{
public:
	wstring				m_strAnimationName;	// 생성된 애니메이션 이름
	wstring				m_strTexturePath;	// 가져올 폴더 파일 경로
	wstring				m_strAnimationType;	// 애니메이션 타입
	AnimationInfo		m_AniInfo;			// 애니메이션 정보

public:
	AnimationData(int _key);
	~AnimationData();

public:
	virtual void SaveData();
	virtual void ParseData(const json& item) override;
};


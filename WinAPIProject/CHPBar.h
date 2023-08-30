#pragma once

class CObject;

class CHPBar
{
private:
	CObject*	m_pOwner;
	Vec2		m_vOffsetPos;	// 오브젝트와의 상대 거리
	Vec2		m_vFinalPos;	// 최종 위치
	
	int			m_iWidth;
	int			m_iHeight;		// 체력바 크기
	
public:
	CHPBar(int _width, int _height, Vec2 _offset);
	CHPBar(const CHPBar& _origin);
	~CHPBar();

	friend class CObject;

public:
	void FinalUpdate();
	void Render(HDC hdc);


};


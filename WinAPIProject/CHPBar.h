#pragma once

class CObject;

class CHPBar
{
private:
	CObject*	m_pOwner;
	Vec2		m_vOffsetPos;	// ������Ʈ���� ��� �Ÿ�
	Vec2		m_vFinalPos;	// ���� ��ġ
	
	int			m_iWidth;
	int			m_iHeight;		// ü�¹� ũ��
	
public:
	CHPBar(int _width, int _height, Vec2 _offset);
	CHPBar(const CHPBar& _origin);
	~CHPBar();

	friend class CObject;

public:
	void FinalUpdate();
	void Render(HDC hdc);


};


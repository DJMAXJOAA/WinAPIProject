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
	bool		m_bMonster;		// �������� �ƴ���
	
public:
	CHPBar(int _width, int _height, Vec2 _offset, bool _monster);
	CHPBar(const CHPBar& _origin);
	~CHPBar();

	friend class CObject;

private:
	void DrawTextWithBorder(Graphics& graphics, const std::wstring& text, const Font& font, const PointF& point);

public:
	void FinalUpdate();
	void Render(HDC hdc);
};


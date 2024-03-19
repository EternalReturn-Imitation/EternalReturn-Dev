#pragma once
#include "CComponent.h"

class CText :
    public CComponent
{
private:
	wstring m_str;
	Vec2	m_vFontPos;
	Vec2	m_vOffsetPos;

	wstring	m_Font;

	float	m_fFontSize;
	UINT	m_FontColor;
	UINT	m_Flags;

public:
	virtual void finaltick() override;

	virtual void begin() override;
	virtual void tick() override;
	
	void InputString(const wstring& _string) { m_str = _string; }
	const wstring& GetString() { return m_str; }

	void TextInit(const wchar_t* _Font, Vec2 _OffsetPos, float _FontSize, UINT _FontColor, UINT _Flags);

	void SetFontColor(UINT _Color) { m_FontColor = _Color; }
	void SetFontColor(UINT _R, UINT _G, UINT _B, UINT _A) { m_FontColor = FONT_RGBA(_R, _G, _B, _A); }
	UINT GetFontColor() { return m_FontColor; }
	


	void SetFont(const wstring& _font) { m_Font = _font; }
	wstring GetFont() { return m_Font; }

	void SetFontSize(float _size) { m_fFontSize = _size; }
	float GetFontSize() { return m_fFontSize; }

	void SetOffsetPos(Vec2 _vPos) { m_vOffsetPos = _vPos; }
	Vec2 GetOffsetPos() { return m_vOffsetPos; }

	void render();

	virtual void SaveToLevelFile(FILE* _File);
	virtual void LoadFromLevelFile(FILE* _FILE);

    CLONE(CText);

public:
    CText();
    CText(const CText& _Origin);
    ~CText();
};


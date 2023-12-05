#pragma once

enum class KEY
{
	UP,		// VK_UP
	DOWN,
	LEFT,
	RIGHT,

	SPACE,
	ENTER,
	ESC,
	LALT,
	LCTRL,
	LSHIFT,
	TAB,

	LBTN,
	RBTN,
	
	Q,
	W,
	E,
	R,

	T,Y,U,I,O,P,

	A,
	S,
	D,
	F,

	Z,
	X,
	C,
	V,

	_0,
	_1,
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
	_8,
	_9,
	
	END,
};

enum class KEY_STATE
{
	TAP,
	PRESSED,
	RELEASE,
	NONE,
};


struct tKeyInfo
{
	KEY			key;
	KEY_STATE	state;
	bool		bPrev;
};




class CKeyMgr 
	: public CSingleton<CKeyMgr>
{
	SINGLE(CKeyMgr);
private:
	vector<tKeyInfo>	m_vecKey;
	Vec2				m_vMousePos;
	Vec2				m_vPrevMousePos;
	Vec2				m_vMouseDir;

public:
	void init();
	void tick();


public:
	KEY_STATE GetKeyState(KEY _key) { return m_vecKey[(UINT)_key].state; }
	Vec2 GetMousePos() { return m_vMousePos; }
	Vec2 GetMouseDir() { return m_vMouseDir; }
};


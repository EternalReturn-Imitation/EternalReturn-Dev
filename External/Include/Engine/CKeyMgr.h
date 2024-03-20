#pragma once

#include <dinput.h>

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

	T, Y, U, I, O, P,

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

	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,

	APOSTROPHE,

	WheelUp,
	WheelDown,
	
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

	Vec2				m_vUnProjectMousePos;


private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_mouse;

	bool mouseWheelUp;
	bool mouseWheelDown;

	DIMOUSESTATE m_mouseState;

public:
	void init();
	void tick();

	void DinputInit(HINSTANCE _hinstance, HWND _hwnd);

	void Release();

public:
	KEY_STATE GetKeyState(KEY _key) { return m_vecKey[(UINT)_key].state; }
	Vec2 GetMousePos() { return m_vMousePos; }
	Vec2 GetMouseDir() { return m_vMouseDir; }
	Vec2 GetUnProjectPos() { return m_vUnProjectMousePos; }

	bool IsMouseWheelUp() { return mouseWheelUp; }
	bool IsMouseWheelDown() { return mouseWheelDown; }

	void CalcUnProjectPos();
};


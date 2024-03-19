#pragma once
#include <Engine\CSingleton.h>

class ER_GameSystem
	: public CSingleton<ER_GameSystem>
{
	SINGLE(ER_GameSystem);
private:
	CGameObject* m_pPlayerCharacter;

public:
	void SetPlayerCharacter(CGameObject* _PC) { m_pPlayerCharacter = _PC; }

public:
	void init();
	void progress();
	void tick();
};


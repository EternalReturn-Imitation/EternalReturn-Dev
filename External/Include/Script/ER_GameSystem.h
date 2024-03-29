#pragma once
#include <Engine\CSingleton.h>

class ER_Cursor;

class ER_GameSystem
	: public CSingleton<ER_GameSystem>
{
	SINGLE(ER_GameSystem);
private:
	CGameObject* m_pPlayerCharacter;
	vector<CGameObject*> m_pCharacters;
	CGameObject* m_pCursor;

public:
	void SetPlayerCharacter(CGameObject* _PC) { m_pPlayerCharacter = _PC; }
	void RegistCurLevelCharacter(CGameObject* _Character) { m_pCharacters.push_back(_Character); }

	CGameObject* GetPlayerCharacter() { return m_pPlayerCharacter; }
	vector<CGameObject*> GetCharacters() { return m_pCharacters; }

	ER_Cursor* GetCursor();

	void GameStart();

public:
	void init();
	void progress();
	void tick();
};


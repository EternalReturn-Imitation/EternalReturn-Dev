#pragma once
#include <Engine\CSingleton.h>

class ER_UIMgr
	: public CSingleton<ER_UIMgr>
{
	SINGLE(ER_UIMgr);
private:
	std::pair<CGameObject*, CGameObject*> m_aInventoryList[2][5];
	std::pair<CGameObject*, CGameObject*> m_aEquipList[2][3];
	CGameObject* m_pPCFace;
	CGameObject* m_pHPBar;
	CGameObject* m_pStemnarBar;
	CGameObject* m_aSkillList[4];
	CGameObject* m_pItemBox;
	std::pair<CGameObject*, CGameObject*> m_aItemBoxList[2][4];

public:
	void init();
	void tick();

public:
	void GameStart();
	void CreateBaseUI();
	void CreateHPBar();
	void CreateSkill();
	void CreateInventorySlot();
	void CreateInventoryItem();
	void CreateEquipSlot();
	void CreateEquipItem();
	void CreateDropInventory();

public:
	Vec3 WorldPosToUIPos(const Vec3& worldPos);
};


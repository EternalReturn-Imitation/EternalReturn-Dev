#pragma once
#include <Engine\CSingleton.h>

class ER_UIMgr :
    public CSingleton<ER_UIMgr>
{
    SINGLE(ER_UIMgr);
private:
    // [ Status Bar ]
    CGameObject* StatusBar_Center;              // 스킬버튼스크립트 / 쿨타임
    CGameObject* StatusBar_CharacterInfo;       // 장비창 버튼스크립트
    CGameObject* StatusBar_Inventory;           // 버튼 스크립트

    // [ HP / SP ]
    CGameObject* StatusBar_Gauge[2];             // 0 : HP, 1 : SP

    // [ Skill ]
    CGameObject* StatusBar_SkillSlot[4];        // 스킬버튼
    CGameObject* StatusBar_SkillLevelUpBtn[4];  // 버튼 스크립트

private:
    void LobbyUI();
    void InGameUI();

    void CreateCenter();
    void CreateCharacterInfo();
    void CreateInventory();

public:
    void init();
    void tick();

    void SpawnUI();

    void RegistPlayerCharacetr();
};


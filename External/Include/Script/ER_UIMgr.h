#pragma once
#include <Engine\CSingleton.h>

class ER_UIMgr :
    public CSingleton<ER_UIMgr>
{
    SINGLE(ER_UIMgr);
private:
    // =====================
    // [ Status Bar Center ]
    // =====================
    CGameObject* StatusBar_Center;              
    
    // [ HP / SP ]
    CGameObject* StatusBar_Gauge[2];             // 0 : HP, 1 : SP
    
    // [ Skill ]
    CGameObject* StatusBar_SkillSlot[4];        // 스킬버튼 / 쿨타임
    CGameObject* StatusBar_SkillLevelUpBtn[4];  // 버튼 스크립트
    
    // ==================
    // [ Character Info ]
    // ==================
    CGameObject* StatusBar_CharacterInfo;                   // 능력치 창 + 보드
    CGameObject* StatusBar_CharacterInfo_Portrait;          // 캐릭터 초상화
    CGameObject* StatusBar_CharacterInfo_Level;             // 레벨
    CGameObject* StatusBar_CharacterInfo_Exp;               // 경험치 바

    CGameObject* StatusBar_CharacterInfo_EquipMent[5];      // 장비창

    // =====================
    // [ Inventory ]
    // =====================
    CGameObject* StatusBar_Inventory;
    CGameObject* StatusBar_Inventory_Slot[10];


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


#pragma once
#include <Engine\CScript.h>
#include "ER_define.h"
#include "ER_struct.h"

class ER_DataScript_Character :
    public CScript
{
private:
    wstring                     m_strKey;
    wstring                     m_strName;              // 실험체 이름
    ER_Initial_Stats            m_STDStats;             // 실험체 초기 능력치

    Ptr<CTexture>               m_PortraitTex;          // 초상화 텍스쳐
    Ptr<CTexture>               m_FullTax;              // 전신 일러
    Ptr<CTexture>               m_MapTex;               // 미니맵 텍스쳐
    vector<ER_SKILL*>           m_SkillList;            // 보유 스킬             

    // 실험체 최종 능력치 (초기능력치 * 레벨, + 아이템, + 스킬효과)
    tIngame_Stats*              m_Stats;                // 게임 능력치
    ER_SKILL*                   m_Skill[4];             // 실제사용스킬

    // 상태판단
    tStatus_Effect*             m_StatusEffect;         // 상태효과 구조체
    bool                        m_bGameDead;            // 캐릭터 사망판단
    bool                        m_bOutofContorl;        // 제어불가상태


    // 장비창
    CGameObject*                m_Equipment[(UINT)ER_ITEM_SLOT::END];   // 장비칸
    CGameObject*                m_Inventory[10];                        // 인벤토리 10칸

    CGameObject*                m_aStatBar[4];          //[0] : HPBAR, [1] : ReturnBar, [2] : SteminarBar, [3] : LevelText
    float                       m_aStatPosOffset[4];    //스탯들의 x 위치 오프셋들.
    bool                        m_bHPChangeTrigger;     //HP 변경시 트리거

public:
    // [상태 갱신]
    void StatusUpdate();    // Status 갱신 함수 : 레벨업 , 아이템변경, 버프/버프
    // 스킬 쿨타임 갱신 함수


public:
    void init();
    virtual void begin() override;
    virtual void tick() override;

    const wstring& GetCharacterName() { return m_strKey; }

public:
    // [Status]
    ER_Ingame_Stats* GetStatus() { return m_Stats; }

    Ptr<CTexture> GetPortraitTex() { return m_PortraitTex; }
    
    tStatus_Effect* GetStatusEffect() { return m_StatusEffect; }
    const bool IsDeadState() { return m_bGameDead; }
    const bool IsOutofControl() { return m_bOutofContorl; }

    // [Skill]
    vector<ER_SKILL*>& GetSkillList() { return m_SkillList; }
    ER_SKILL* GetSkill(int _Idx) { return m_SkillList[_Idx]; }

    // [Item]
    CGameObject** GetAllEquipItem() { return m_Equipment; }
    CGameObject* GetEquipItem(UINT _SlotType) { return m_Equipment[_SlotType]; }

    CGameObject** GetAllInvenItem() { return m_Inventory; }
    CGameObject* GetInvenItem(UINT _SlotX, UINT _SlotY) { return m_Equipment[_SlotX * 5 + _SlotY]; }
    CGameObject* GetInvenItem(UINT _SlotType) { return m_Equipment[_SlotType]; }

    CGameObject* ItemAcquisition(CGameObject* _ItemObj);

    bool SwapItem(CGameObject* _DragmItem, CGameObject* _DropItem);

public:
    void CreateStatBar();
    void UpdateStatBar(); // 매 틱 업데이트
    void ChangeStatBar(); // 변환이 필요할 때만 호출
    void ChangeHPReturnBar(); //HPReturnBar 변경시 호출

public:
    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void OnOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;

    virtual void BeginRayOverlap() override;
    virtual void OnRayOverlap() override;
    virtual void EndRayOverlap() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

public:
    ER_DataScript_Character();
    ER_DataScript_Character(const ER_DataScript_Character& _origin);
    ~ER_DataScript_Character();
    
    CLONE(ER_DataScript_Character);

    friend class ER_CharacterMgr;
    friend class CharacterDataUI;
};


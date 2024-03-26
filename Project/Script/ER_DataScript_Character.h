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

    UINT                        m_SkillPoint;           // 스킬 레벨업 투자 가능 포인트
    float                       m_fSPRegenTime;         // SPRegen Tiem

    // 상태판단
    tStatus_Effect*             m_StatusEffect;         // 상태효과 구조체
    bool                        m_bGameDead;            // 캐릭터 사망판단
    bool                        m_bOutofContorl;        // 제어불가상태


    // 장비창
    CGameObject*                m_Equipment[(UINT)ER_ITEM_SLOT::END];   // 장비칸
    // 인벤토리
    CGameObject*                m_Inventory[10];                        // 인벤토리 10칸

public:
    // [상태 갱신]
    void                        StatusUpdate();                            // Status 갱신 함수 : 레벨업 , 아이템변경, 버프/버프
    void                        HPRegen(float _magnification = 1.f);      // 자연체력회복. 기본 1배율
    void                        SPRegen(float _magnification = 1.f);      // 자연스테미너회복. 기본 1배율

    void                        LevelUP();                                 // 레벨업 시 동작하는 함수 집합

public:
    void init();
    virtual void begin() override;
    virtual void tick() override;

    const wstring&      GetCharacterName() { return m_strKey; }
    const wstring&      GetCharacterKorName() { return m_strName; }

public:
    // [Status]
    ER_Ingame_Stats*    GetStatus() { return m_Stats; }
    UINT*               GetSkillPoint() { return &m_SkillPoint; }

    Ptr<CTexture>       GetPortraitTex() { return m_PortraitTex; }
    
    tStatus_Effect*     GetStatusEffect() { return m_StatusEffect; }
    void                SetGameDead() { m_bGameDead = true; }
    const bool          IsDeadState() { return m_bGameDead; }
    const bool          IsOutofControl() { return m_bOutofContorl; }

    // [Skill]
    vector<ER_SKILL*>&  GetSkillList() { return m_SkillList; }
    ER_SKILL*           GetSkill(int _Idx) { return m_Skill[_Idx]; }
    ER_SKILL**          GetSkillAdress(int _Idx) { return &m_Skill[_Idx]; }
    
    void                ChangeSkill(int _Idx);
    void                SkillSlotInit();

    // [Item]
    CGameObject**       GetAllEquipItem() { return m_Equipment; }
    CGameObject*        GetEquipItem(UINT _SlotType) { return m_Equipment[_SlotType]; }

    CGameObject**       GetAllInvenItem() { return m_Inventory; }
    CGameObject*        GetInvenItem(UINT _SlotX, UINT _SlotY) { return m_Equipment[_SlotX * 5 + _SlotY]; }
    CGameObject*        GetInvenItem(UINT _SlotType) { return m_Equipment[_SlotType]; }

    CGameObject*        ItemAcquisition(CGameObject* _ItemObj);

    bool                SwapItem(CGameObject** _DragItem, CGameObject** _DropItem);
    void                AcquireItem(CGameObject** _BoxSlot);  // 목적지슬롯
    void                CraftItem(CGameObject** _iSlot1, CGameObject** _iSlot2);    // 조합, 재료슬롯,재료슬롯

public:
    virtual void        BeginOverlap(CCollider3D* _Other) override;
    virtual void        OnOverlap(CCollider3D* _Other) override;
    virtual void        EndOverlap(CCollider3D* _Other) override;

    virtual void        BeginRayOverlap() override;
    virtual void        OnRayOverlap() override;
    virtual void        EndRayOverlap() override;

public:
    virtual void        SaveToLevelFile(FILE* _File) override;
    virtual void        LoadFromLevelFile(FILE* _FILE) override;

public:
    ER_DataScript_Character();
    ER_DataScript_Character(const ER_DataScript_Character& _origin);
    ~ER_DataScript_Character();
    
    CLONE(ER_DataScript_Character);


    friend class ER_CharacterMgr;
    friend class CharacterDataUI;
};


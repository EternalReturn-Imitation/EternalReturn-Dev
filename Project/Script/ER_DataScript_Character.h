#pragma once
#include <Engine\CScript.h>
#include "ER_define.h"
#include "ER_struct.h"

class ER_DataScript_Character :
    public CScript
{
private:
    // [ Info ]
    wstring                     m_strKey;
    wstring                     m_strName;              // 실험체 이름
    ER_Initial_Stats            m_STDStats;             // 실험체 초기 능력치

    Ptr<CTexture>               m_PortraitTex;          // 초상화 텍스쳐
    Ptr<CTexture>               m_FullTax;              // 전신 일러
    Ptr<CTexture>               m_MapTex;               // 미니맵 텍스쳐
    Ptr<CSound>                 m_LevelUpSound;         // 레벨업 사운드
    vector<ER_SKILL*>           m_SkillList;            // 보유 스킬             

    // [ Stats ]
    tIngame_Stats*              m_Stats;                // 게임 능력치

    // [ Skill ]
    UINT                        m_SkillPoint;           // 스킬 레벨업 투자 가능 포인트
    float                       m_fSPRegenTime;         // SPRegen Tiem

    // [ Status Effect ]
    tStatus_Effect*             m_StatusEffect;         // 상태효과 구조체
    bool                        m_bGameDead;            // 캐릭터 사망판단
    bool                        m_bOutofContorl;        // 제어불가상태

    // [ Item ]
    CGameObject*                m_Equipment[(UINT)ER_ITEM_SLOT::END];   // 장비창
    CGameObject*                m_Inventory[10];                        // 인벤토리
    UINT                        m_RootItem[5];                          // 최종 목표 아이템
    vector<UINT>                m_CraftList;                            // 제작가능 아이템 목록

    // [ Debug / CoolDown Delete ]
    bool                        DebugMode;
    unordered_map<UINT, int>    m_IngredientList;                       // 재료 아이템 목록
    unordered_map<UINT, int>    m_NeedFarmingItems;                     // 필요 파밍 아이템 목록

    mutex                       m_mMutex;

    // [ Developer cheat ]
    bool                        bCoolDownCheat;
    bool                        bInvincibleCheat;

public:
    // [상태 갱신]
    void                        StatusUpdate();                            // Status 갱신 함수 : 레벨업 , 아이템변경, 버프/버프
    void                        HPRegen(float _magnification = 1.f);      // 자연체력회복. 기본 1배율
    void                        SPRegen(float _magnification = 1.f);      // 자연스테미너회복. 기본 1배율

    void                        LevelUP();                                 // 레벨업 시 동작하는 함수 집합

public:
    void         init();
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
    ER_SKILL*           GetSkill(int _Idx) { return m_SkillList[_Idx]; }
    ER_SKILL**          GetSkillAdress(int _Idx) { return &m_SkillList[_Idx]; }

    // [Item]
    CGameObject**       GetAllEquipItem() { return m_Equipment; }
    CGameObject**       GetEquipItem(UINT _SlotType) { return &m_Equipment[_SlotType]; }
    vector<UINT>*       GetCraftListAdress() { return &m_CraftList; }

    CGameObject**       GetAllInvenItem() { return m_Inventory; }
    CGameObject*        GetInvenItem(UINT _SlotX, UINT _SlotY) { return m_Equipment[_SlotX * 5 + _SlotY]; }
    CGameObject*        GetInvenItem(UINT _SlotType) { return m_Equipment[_SlotType]; }

    unordered_map<UINT, int> GetIngredientList() { return m_IngredientList; }
    unordered_map<UINT, int> GetNeedFarmingList() { return m_NeedFarmingItems; }

    void                SetRootItem(UINT* _RootItem, int _cnt)
    {
        for (int i = 0; i < _cnt; ++i)
        {
            m_RootItem[i] = _RootItem[i];
        }
    }
    UINT*               GetRootItem() { return m_RootItem; }

    bool                SwapItem(CGameObject** _DragItem, CGameObject** _DropItem);
    void                AcquireItem(CGameObject** _BoxSlot);  // 목적지슬롯
    void                ItemInfoUpdate();
    bool                CraftItem(UINT _Item);    // 조합, 재료슬롯,재료슬롯

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


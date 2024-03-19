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

    // 실험체 최종 능력치 (초기능력치 * 레벨, + 아이템, + 스킬효과)
    tIngame_Stats*    m_Stats;                // 게임 능력치
    vector<ER_SKILL*>           m_Skill;                // 보유 스킬             

    // [인벤토리 15개]
    // 장착 5개
    // 인벤토리전용 10개

    // [함수]
    // 능력치 갱신 함수 (아이템 장착, 레벨업, 상태이상 등 상호작용시 마지막에 호출)
    void StatusUpdate();    // Status 갱신 함수

    vector<ER_SKILL*>& GetSkillList() { return m_Skill; }

    CLONE(ER_DataScript_Character);
public:
    void init();
    virtual void begin() override;
    virtual void tick() override;

public:
    ER_Ingame_Stats* GetStatus() { return m_Stats; }
    const ER_SKILL* GetSkill(int _Idx) { return m_Skill[_Idx]; }


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

    friend class ER_CharacterMgr;
    friend class CharacterDataUI;
};


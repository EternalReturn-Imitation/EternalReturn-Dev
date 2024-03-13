#pragma once
#include <Engine\CScript.h>
#include "ER_define.h"
#include "ER_struct.h"

class ER_Data_CharacterScript :
    public CScript
{
private:
    wstring             m_strKey;
    wstring             m_strName;              // 실험체 이름
    ER_STD_CHAR_STATS   m_STDStats;             // 실험체 초기 능력치

    Ptr<CTexture>       m_PortraitTex;          // 초상화 텍스쳐
    Ptr<CTexture>       m_FullTax;              // 전신 일러
    Ptr<CTexture>       m_MapTex;               // 미니맵 텍스쳐

    // 실험체 최종 능력치 (초기능력치 * 레벨, + 아이템, + 스킬효과)


    // [인벤토리 15개]
    // 장착 5개
    // 인벤토리전용 10개

    // [함수]
    // 능력치 갱신 함수 (아이템 장착, 레벨업, 상태이상 등 상호작용시 마지막에 호출)

    CLONE(ER_Data_CharacterScript);
public:
    void init();
    virtual void begin() override;
    virtual void tick() override;


public:
    ER_Data_CharacterScript();
    ~ER_Data_CharacterScript();

    friend class ER_CharacterMgr;
    friend class CharacterDataUI;
};


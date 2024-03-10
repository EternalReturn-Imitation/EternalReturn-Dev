#pragma once
#include <Engine\CGameObject.h>

class ER_Character :
    public CGameObject
{
private:
    // 에디터로 설정해놓을 것.
    wstring m_strCharacterKey;  // Character Key
    wstring m_strCharacterName; // Character Name Kr

    // 메시데이터
    // =========[속성값]========
    ER_Std_Char_Stats m_tStd_Char_Stats;      // 기본 캐릭터 스탯 (기준값)
    // 최종 스탯 (레벨 + 아이템 + 능력치 값)
    // ========================

    // 인게임 스탯
    // =========
    ER_Ingame_Stats m_tIngame_Stats;    // 인게임 스탯
    // 버프/디버프 값
    
    // ======[동작별 함수]======
    // 일반

    // 기본 공격
    // 스킬 Q
    // 스킬 W
    // 스킬 E
    // 스킬 R
    
    // 상자
    // 채집
    
    // 제작

    // 스폰
    // 사망
    
    // 휴식

    // 이동
    // ========================


public:
    void begin();

    void tick();
    virtual void finaltick();
    void finaltick_module();
    void render();
    void render_shadowmap();

};
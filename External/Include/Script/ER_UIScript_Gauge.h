#pragma once
#include <Engine\CScript.h>

class ER_DataScript_Character;

class ER_UIScript_Gauge :
    public CScript
{
private:
    ER_DataScript_Character*    m_Data;
    UINT                        m_GaugeType;        // HP : 0, SP : 1

    tIngame_Stats*              m_Stats;

    float                       m_PrevValue;

    float                       m_MainRatio;
    float                       m_ReturnRatio;

public:
    void RegistCharacetr(ER_DataScript_Character* _CharacterData);

    float Lerp(float CurValue, float PrevValue);

public:
    void init();
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetUI(int type);

public:
    ER_UIScript_Gauge();
    ~ER_UIScript_Gauge();

    CLONE(ER_UIScript_Gauge);
};


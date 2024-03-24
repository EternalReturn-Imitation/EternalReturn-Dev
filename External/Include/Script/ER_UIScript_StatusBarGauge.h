#pragma once
#include <Engine\CScript.h>
#include "ER_DataScript_Character.h"

class ER_UIScript_StatusBarGauge :
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
    void RegistPlayerCharacetr();

    float Lerp(float CurValue, float PrevValue);

public:
    void init();
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetUI(int type);

public:
    ER_UIScript_StatusBarGauge();
    ~ER_UIScript_StatusBarGauge();

    CLONE(ER_UIScript_StatusBarGauge);
};


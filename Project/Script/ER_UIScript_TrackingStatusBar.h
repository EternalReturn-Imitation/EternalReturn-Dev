#pragma once
#include <Engine\CScript.h>

class ER_UIScript_TrackingStatusBar :
    public CScript
{
private:
    tIngame_Stats*  m_CharaterData;
    int             m_iLevel;
    CGameObject*    Board;
    CGameObject*    HPGuage;
    CGameObject*    SPGuage;

public:
    void init(CGameObject* _CharacterObj);

    virtual void begin() override;
    virtual void tick() override;

public:
    void TrackingCharacterPos();

public:
    ER_UIScript_TrackingStatusBar();
    ~ER_UIScript_TrackingStatusBar();

    CLONE(ER_UIScript_TrackingStatusBar);
};


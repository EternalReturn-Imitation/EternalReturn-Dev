#pragma once
#include <Engine\CScript.h>

class ER_DataScript_Character;

class ER_PlayerScript_Range :
    public CScript
{
private:
    CGameObject* m_pOwner;
    ER_DataScript_Character* m_CharacterData;
    bool m_bIsDecal;

public:
    void init();
    virtual void begin() override;
    virtual void tick() override;

    void SetOwnerObj(CGameObject* _owner);

public:
    ER_PlayerScript_Range();
    ~ER_PlayerScript_Range();

    CLONE_DISABLE(ER_PlayerScript_Range);
};


#pragma once
#include "UI.h"
#include <Script\ER_struct.h>

class CharacterDataUI :
    public UI
{
private:
    map<wstring, CGameObject*>* m_pCharacters;
  
    CGameObject*        m_pSelectedCharacter;
    int                 m_iSelectedCharIdx;
    
    tSkill_Info*        m_pSelectedSkill;
    int                 m_iSelectedSkillIdx;

public:
    void RegistCharacters();

public:
    virtual void init() override;
    virtual void tick() override;
    virtual void finaltick() override;
    virtual int render_update() override;

    void render_menubar();
    void render_CharacterList();
    void render_CharacterInfoData();
    void render_SkillList();
    void render_SkillInfoData();

public:
    CharacterDataUI();
    virtual ~CharacterDataUI();

};


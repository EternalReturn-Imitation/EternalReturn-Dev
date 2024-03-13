#pragma once
#include "UI.h"

class CharacterDataUI :
    public UI
{
private:
    map<wstring, CGameObject*>* m_pCharacters;
  
    CGameObject* m_pSelectedCharacter;

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
    void render_ActionFuncLink();

public:
    CharacterDataUI();
    virtual ~CharacterDataUI();

};


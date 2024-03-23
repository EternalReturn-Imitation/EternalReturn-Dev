#pragma once
#include "CUIComponent.h"
#include "ptr.h"

class CTexture;

class CUIComponent :
    public CUIComponent
{
private:
    enum eButtonState
    {
        IDLE,
        OVER,
        PRESSED,
        END,
    };

private:
    Ptr<CTexture> m_pIdleTex;
    Ptr<CTexture> m_pOverTex;
    Ptr<CTexture> m_pPressedTex;

public:
    Ptr<CTexture> m_pOriginTex;

public:
    virtual void begin();
    virtual void tick();
    virtual void finaltick();

public:
    Ptr<CTexture> GetOrigintex() { return m_pOriginTex; }

public:
    virtual void SaveToLevelFile(FILE* _File);
    virtual void LoadFromLevelFile(FILE* _FILE);

    CLONE(CUIComponent);

public:
    CUIComponent();
    CUIComponent(const CUIComponent& _Other);
    virtual ~CUIComponent();

};


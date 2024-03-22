#pragma once
#include "CUIComponent.h"
#include "ptr.h"

class CTexture;

class CUI_Button :
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

    CLONE(CUI_Button);

public:
    CUI_Button();
    CUI_Button(const CUI_Button& _Other);
    virtual ~CUI_Button();

};


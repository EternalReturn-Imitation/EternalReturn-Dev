#pragma once
#include "CComponent.h"

class CScript;

typedef void (CScript::* CUI_DELEGATE)(void);

class CUIComponent :
    public CComponent
{
private:
    bool m_bCsrPressed;
    bool m_bCsrOnUI;

    bool bOpen;

private:
    CScript* m_pUIScript;

    CUI_DELEGATE m_CsrOnFunc;
    CUI_DELEGATE m_CsrTapFunc;
    CUI_DELEGATE m_CsrReleasFunc;
    CUI_DELEGATE m_CsrClickFunc;

private:
    void CsrOnCheck();

public:
    virtual void begin();
    virtual void tick() = 0;
    virtual void finaltick();

public:
    bool IsCsrOn() { return m_bCsrOnUI; }
    bool IsCsrPressed() { return m_bCsrPressed; }

public:
    void SetUIScript(CScript* _UIScript) { m_pUIScript = _UIScript; }
    
    void SetUIFunc_CsrOn(CUI_DELEGATE _func) { m_CsrOnFunc = _func; }
    void SetUIFunc_CsrTap(CUI_DELEGATE _func) { m_CsrTapFunc = _func; }
    void SetUIFunc_CsrRelease(CUI_DELEGATE _func) { m_CsrReleasFunc = _func; }
    void SetUIFunc_CsrClick(CUI_DELEGATE _func) { m_CsrClickFunc = _func; }

    void CsrOn();
    void CsrTap();
    void CsrRelease();
    void CsrClick();

public:
    virtual void SaveToLevelFile(FILE* _File) {}
    virtual void LoadFromLevelFile(FILE* _FILE) {}

public:
    CUIComponent(COMPONENT_TYPE _type);
    virtual ~CUIComponent();
};


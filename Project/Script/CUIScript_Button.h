#pragma once
#include <Engine\CScript.h>

class CUIScript_Button :
    public CScript
{
private:
    enum eBtnState
    {
        IDLE,
        ON,
        DOWN,
        END
    };

    enum UICase {
        BOXITEM,
        INVENITEM,
        EQUIPITEM,
        UICASEEND,
    };

    enum eBtnCase {
        INVENBUTTON,
        BOXBUTTON,
        BTNCASEEND,
    };

public:
    int m_iUIPos;
    UICase m_eUICase;
    eBtnCase m_eBtnCase;

public:
    void init();
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetUIPos(int _x) { m_iUIPos = _x; }
    void SetUICase(int _case) { m_eUICase = (UICase)_case; }
    void SetBtnCase(int _case) { m_eBtnCase = (eBtnCase)_case; }

public:
    void ItemAcquisiton();

public:
    CUIScript_Button();
    CUIScript_Button(const CUIScript_Button& _origin);
    ~CUIScript_Button();

    CLONE(CUIScript_Button);
};
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

public:

public:
    void init();
    virtual void begin() override;
    virtual void tick() override;

public:
    void Calculate() { int a = 0; }

public:
    CUIScript_Button();
    CUIScript_Button(const CUIScript_Button& _origin);
    ~CUIScript_Button();

    CLONE(CUIScript_Button);
};
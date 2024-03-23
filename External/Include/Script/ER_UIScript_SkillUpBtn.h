#pragma once
#include <Engine\CScript.h>

class ER_UIScript_SkillUpBtn :
    public CScript
{
private:

public:
    void init();
    virtual void begin() override;
    virtual void tick() override;

public:
    ER_UIScript_SkillUpBtn();
    ~ER_UIScript_SkillUpBtn();

    CLONE(ER_UIScript_SkillUpBtn);
};


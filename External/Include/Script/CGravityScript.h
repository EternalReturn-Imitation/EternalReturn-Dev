#pragma once
#include <Engine\CScript.h>
class CGravityScript :
    public CScript
{
public:
    virtual void tick() override;

private:
    CLONE(CGravityScript);
public:
    CGravityScript();
    ~CGravityScript();
};


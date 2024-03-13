#pragma once
#include <Engine\CScript.h>

class ER_PlayerScript :
    public CScript
{
private:

private:
    void MoveToPoint();

public:
    void init();
    virtual void begin() override;
    virtual void tick() override;

public:
    ER_PlayerScript();
    ~ER_PlayerScript();

    CLONE_DISABLE(ER_PlayerScript);
};


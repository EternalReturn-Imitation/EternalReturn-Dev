#pragma once
#include "Engine\CScript.h"

class CTestScript :
    public CScript
{
private:

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;
    CLONE(CTestScript);

    CTestScript();
    ~CTestScript();
};


#pragma once
#include "Engine\CScript.h"

class CCloudScript :
    public CScript
{
private:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    CLONE(CCloudScript);

public:
    CCloudScript();
    ~CCloudScript();

};


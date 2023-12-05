#pragma once
#include <Engine\CScript.h>

class CMonsterScript :
    public CScript
{
private:

public:
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;

    CLONE(CMonsterScript);
public:
    CMonsterScript();
    ~CMonsterScript();
};



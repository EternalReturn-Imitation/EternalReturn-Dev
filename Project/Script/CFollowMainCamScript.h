#pragma once
#include <Engine\CScript.h>

class CFollowMainCamScript :
    public CScript
{
private:
    CGameObject* pTargetObject;
    Vec3         OffsetPos;

public:
    virtual void begin() override;
    virtual void tick() override;
    void SetTarget(Vec3 _pos = Vec3(0.f,0.f,0.f))
    { 
        OffsetPos = _pos;
    }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CFollowMainCamScript);

    CFollowMainCamScript();
    ~CFollowMainCamScript();
};


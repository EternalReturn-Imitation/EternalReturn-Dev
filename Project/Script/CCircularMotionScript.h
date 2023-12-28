#pragma once
#include <Engine\CScript.h>

class CCircularMotionScript :
    public CScript
{
private:
    CGameObject* m_pParentPlanet;
    float m_fRadius;
    float m_fHertz;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;
    CLONE(CCircularMotionScript);

public:
    CCircularMotionScript();
    ~CCircularMotionScript();

};


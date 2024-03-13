#pragma once
#include "CComponent.h"
class CFindPath :
    public CComponent
{
private:
    bool LaycastResultTrigger;

private:
    vector<Vec3>        m_vecPath;
    int                 m_iPathCount;
    int                 m_iCurPathIdx;

    float               m_fPrevDir;

    Vec3                m_vNextPos;
    Vec3                m_vPrevEndPos;

private:
    float GetFrontDir(Vec3 _Direction);

public:
    CFindPath();
    ~CFindPath();

public:
    virtual void begin();
    virtual void tick();
    virtual void finaltick();

public:
    void FindPath(Vec3 endPos);
    void FindNextPath();
    bool PathMove(float _fSpeed, bool _IsRotation);

public:
    Vec3 GetNextPos() { return m_vNextPos; }
    void ClearPath();

public:
    virtual void SaveToLevelFile(FILE* _File);
    virtual void LoadFromLevelFile(FILE* _FILE);

    CLONE(CFindPath);
};


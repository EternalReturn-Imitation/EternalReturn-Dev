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
    float               m_fDestDir;

    Vec3                m_vNextPos;
    Vec3                m_vPrevEndPos;

private:
    float GetFrontDir(Vec3 _Direction);
    bool IsLeft(const Vec3& _objDir, const Vec3& _DestDir);

public:
    CFindPath();
    ~CFindPath();

public:
    virtual void begin();
    virtual void tick();
    virtual void finaltick();

public:
    bool FindPath(Vec3 endPos);
    bool FindNextPath();
    bool PathMove(float _fSpeed, bool _isRotation = true);

    Vec3 findMaxClearPoint(const Vec3& _dir, float _min, float _max);
    float findMaxClearDistance(const Vec3& _dir, float _min, float _max);
    float findMaxClearDistanceByWall(const Vec3& _dir, float _min, float _max);

public:
    Vec3 GetNextPos() { return m_vNextPos; }
    void ClearPath();

public:
    virtual void SaveToLevelFile(FILE* _File);
    virtual void LoadFromLevelFile(FILE* _FILE);

    CLONE(CFindPath);
};


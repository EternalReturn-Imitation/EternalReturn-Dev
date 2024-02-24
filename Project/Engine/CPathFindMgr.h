#pragma once
#include "CSingleton.h"

#include "Detour/DetourNavMesh.h"
#include "Detour/DetourNavMeshQuery.h"

class dtNavMesh;
class dtNavMeshQuery;
//class dtPolyRef;
//class dtNavMeshParams;
//class dtTileRef;

struct NavMeshSetHeader
{
    int magic;
    int version;
    int numTiles;
    dtNavMeshParams params;
};

struct NavMeshTileHeader
{
    dtTileRef tileRef;
    int dataSize;
};

static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
static const int NAVMESHSET_VERSION = 1;

#define MAX_POLY 256 


class CPathFindMgr :
    public CSingleton<CPathFindMgr>
{
    SINGLE(CPathFindMgr);

private:
    dtNavMesh*              m_NavMesh;
    dtNavMeshQuery*         m_NavQuery;
    CGameObject*            m_Object;

private:
    tNaviResult         m_sResultPos;

private:
    vector<Vec4>            m_vNaviVtx;
   
private:
    bool LoadNavMeshFromFile(const char* path);
  

public:
    void init();
    void tick();
    void render();


public:
    vector<Vec3> FindPath(const Vec3& startPos, const Vec3& endPos);

public:
    void SetNaviMapObject(CGameObject* _object) { m_Object = _object; }
    void SetNaviResult(tNaviResult _result) { m_sResultPos = _result; }
    void SetNaviVtx(vector<Vec4> _naviVtx) { m_vNaviVtx = _naviVtx; }

    CGameObject* GetNaviMapObject() { return m_Object; }
    tNaviResult GetNaviResult() { return m_sResultPos; }
    const vector<Vec4> GetNaviVtx() { return m_vNaviVtx; }
};


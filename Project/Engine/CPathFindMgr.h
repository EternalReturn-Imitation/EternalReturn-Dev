#pragma once
#include "CSingleton.h"

#include "Detour/DetourNavMesh.h"
#include "Detour/DetourNavMeshQuery.h"

using namespace std;

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
    CGameObject*            m_MapCollider;

private:
    tNaviResult         m_sResultPos;

private:
    vector<Vec4>                    m_vNaviVtx;
    map<pair<float, float>, float>  m_mNaviMap;

private:
    Vec3 m_vNavMeshScale; //현재 네비메쉬의 크기

    bool RayResultTrigger;

   
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
    void SetNavMeshScale(Vec3 _scale) { m_vNavMeshScale = _scale; }
    void SetRayResultTrigger(bool _trigger) { RayResultTrigger = _trigger; }
    void SetMapCollider(CGameObject* _object) { m_MapCollider = _object; }

    CGameObject* GetNaviMapObject() { return m_Object; }
    tNaviResult GetNaviResult() { return m_sResultPos; }
    const vector<Vec4> GetNaviVtx() { return m_vNaviVtx; }
    bool GetRayResultTrigger() { return RayResultTrigger; }
    CGameObject* GetMapCollider() { return m_MapCollider; }
};


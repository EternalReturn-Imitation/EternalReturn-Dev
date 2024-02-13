#include "pch.h"
#include "CPathFindMgr.h"
#include "CPathMgr.h"
#include "CTimeMgr.h"

#include "CTransform.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CLevel.h"
#include "CEventMgr.h"

#include "Detour/DetourNavMesh.h"
#include "Detour/DetourNavMeshQuery.h"

CPathFindMgr::CPathFindMgr()
	: m_NavMesh(nullptr)
	, m_NavQuery(nullptr)
{
}

CPathFindMgr::~CPathFindMgr()
{
	if (m_NavMesh)
	{
		dtFreeNavMesh(m_NavMesh);
		m_NavMesh = nullptr;
	}
	if (m_NavQuery)
	{
		dtFreeNavMeshQuery(m_NavQuery);
		m_NavQuery = nullptr;
	}
}

void CPathFindMgr::init()
{
	LoadNavMeshFromFile("navmesh\\solo_navmesh.bin");
}

void CPathFindMgr::tick()
{

}

void CPathFindMgr::render()
{
}


bool CPathFindMgr::LoadNavMeshFromFile(const char* path)
{
	// NavMesh 파일 경로로부터 불러오기
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	wstring wpath = converter.from_bytes(path);
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += wpath;

	FILE* fp = nullptr;
	errno_t err = _wfopen_s(&fp, strFilePath.c_str(), L"rb");
	if (err != 0 || !fp)
	{
		return 0;
	}

	// NavMesh 헤더 읽어오기
	NavMeshSetHeader header;
	fread(&header, sizeof(NavMeshSetHeader), 1, fp);
	if (header.magic != NAVMESHSET_MAGIC)
	{
		fclose(fp);
		return false;
	}
	if (header.version != NAVMESHSET_VERSION)
	{
		fclose(fp);
		return false;
	}

	// NavMesh 초기화
	m_NavMesh = dtAllocNavMesh();
	if (!m_NavMesh)
	{
		fclose(fp);
		return false;
	}

	dtStatus status = m_NavMesh->init(&header.params);
	if (dtStatusFailed(status))
	{
		fclose(fp);
		return false;
	}

	// NavMesh의 타일 정보 읽어오기 (저희는 타일 옵션을 사용하진 않습니다)
	for (int i = 0; i < header.numTiles; ++i)
	{
		NavMeshTileHeader tileHeader;
		fread(&tileHeader, sizeof(tileHeader), 1, fp);
		if (!tileHeader.tileRef || !tileHeader.dataSize)
			break;

		unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
		if (!data) break;
		memset(data, 0, tileHeader.dataSize);
		fread(data, tileHeader.dataSize, 1, fp);

		m_NavMesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
	}

	// NavQuery 초기화
	m_NavQuery = dtAllocNavMeshQuery();
	if (!m_NavQuery)
	{
		dtFreeNavMesh(m_NavMesh);
		m_NavQuery = nullptr;
		fclose(fp);
		return false;
	}
	m_NavQuery->init(m_NavMesh, 2048);

	return true;
}

vector<Vec3> CPathFindMgr::FindPath(const Vec3& startPos, const Vec3& endPos)
{
	// 시작 위치와 끝 위치를 설정합니다.
	float startpos[3] = { startPos.x, startPos.y, -startPos.z }; // 시작 위치
	float endpos[3] = { endPos.x, endPos.y, -endPos.z }; // 끝 위치

	dtPolyRef startRef, endRef;
	float polyPickExt[3] = { 6000,6000,6000 }; // 범위를 제한하기 위한 벡터

	dtQueryFilter filter;
	filter.setIncludeFlags(0xFFFF); // 모든 폴리곤 참조
	filter.setExcludeFlags(0);      // 제외할 폴리곤 없음

	// 지형 종류별(땅, 물, 벽 등)로 가중치를 주는 부분인데 저희 메쉬에는 지형 종류 설정이 되어있지 않습니다.
	//filter.setAreaCost(1, 1.0f); // Set cost for ground area.
	//filter.setAreaCost(2, 10.0f); // Set high cost for water area.
	//filter.setAreaCost(3, FLT_MAX); // Set infinite cost for wall area.

	// 가까운 폴리곤 검색
	m_NavQuery->findNearestPoly(startpos, polyPickExt, &filter, &startRef, 0);
	m_NavQuery->findNearestPoly(endpos, polyPickExt, &filter, &endRef, 0);

	// 시작과 끝 위치를 찾습니다.
	float nearestStartPos[3], nearestEndPos[3];
	m_NavQuery->closestPointOnPoly(startRef, startpos, nearestStartPos, 0);
	m_NavQuery->closestPointOnPoly(endRef, endpos, nearestEndPos, 0);

	// 경로를 계획합니다.
	dtPolyRef path[MAX_POLY];
	int pathCount;
	m_NavQuery->findPath(startRef, endRef, nearestStartPos, nearestEndPos, &filter, path, &pathCount, 256);

	// 경로를 따라 실제 이동 경로를 생성합니다.

	float* actualPath = new float[3 * MAX_POLY];
	int actualPathCount;
	m_NavQuery->findStraightPath(nearestStartPos, nearestEndPos, path, pathCount, actualPath, 0, 0, &actualPathCount, MAX_POLY);

	// Vec3 형태의 경로를 생성합니다.
	vector<Vec3> vecPath(actualPathCount);
	for (int i = 0; i < actualPathCount; ++i)
	{
		vecPath[i] = Vec3(actualPath[3 * i], actualPath[3 * i + 1], -actualPath[3 * i + 2]);
	}

	delete[] actualPath; // 더이상 필요없는 calcPath를 삭제합니다.

	return vecPath;
}
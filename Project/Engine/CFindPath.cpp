#include "pch.h"
#include "CFindPath.h"

// #include "CNaviMap.h"
#include "CLevelMgr.h"
#include "CGameObject.h"
#include "CKeyMgr.h"
#include "CPathFindMgr.h"
#include "CTransform.h"
#include "CTimeMgr.h"
#include "CLevel.h"
#include "CRenderMgr.h"
#include "CCamera.h"

#define DIR_ERROR_RANGE 0.0872665f	// 오차범위 5도 (radian)
#define DIR_ADD_RADIAN	0.0523599f	// 변화 각도 3도 (radian)


CFindPath::CFindPath()
	:CComponent(COMPONENT_TYPE::FINDPATH)
	, LaycastResultTrigger(false)
	, m_vNextPos(Vec3(NaN, NaN, NaN))
	, m_iPathCount(0)
	, m_iCurPathIdx(0)
	, m_fPrevDir(0.f)
	, m_fDestDir(0.f)
{
}

CFindPath::~CFindPath()
{
}

void CFindPath::begin()
{
}

void CFindPath::tick()
{
}

void CFindPath::finaltick()
{
}

bool CFindPath::FindPath(Vec3 endPos)
{
	// 거리판단용
	// float dist = (endPos - m_vPrevEndPos).Length();

	// 목적지 경로 초기화
	m_vecPath.clear();
	
	// 오브젝트의 현재 위치
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

	// 현재 경로를 시작으로 경로를 받아옴
	m_vecPath = CPathFindMgr::GetInst()->FindPath(CurPos, endPos);
	m_iPathCount = (UINT)m_vecPath.size();
	m_iCurPathIdx = 0;

	if (m_iPathCount <= 1)
	{
		m_vecPath.clear();
		m_vNextPos = Vec3(NaN, NaN, NaN);
		m_vPrevEndPos = endPos;
		return true;
	}

	m_vNextPos = Vec3(m_vecPath[1].x, m_vecPath[1].y, m_vecPath[1].z);

	// 목적지 방향 저장
	Vec3 Dir = (m_vNextPos - CurPos).Normalize();
	m_fDestDir = GetFrontDir(Dir);

	m_vPrevEndPos = endPos;

	return true;
}

bool CFindPath::FindNextPath()
{
	m_iCurPathIdx++;

	// 다음 경로가 있다면 그 위치를 반환하고, 경로 인덱스 증가시킴
	if (m_iCurPathIdx != -1 && m_iCurPathIdx < m_iPathCount)
	{
		m_vNextPos = Vec3(m_vecPath[m_iCurPathIdx].x, m_vecPath[m_iCurPathIdx].y, m_vecPath[m_iCurPathIdx].z);

		// 다음이동경로가 있다.
		Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
		Vec3 vNextDir = (m_vNextPos - CurPos).Normalize();	// 새로운 목적지 방향
		m_fDestDir = GetFrontDir(vNextDir);

		return true;
	}
	// 다음 경로가 없다면 갈 수 없는 위치 반환
	else
	{
		m_vNextPos = Vec3(NaN, NaN, NaN);
		m_fDestDir = 0.f;

		return false;
	}
}

bool CFindPath::PathMove(float _fSpeed)
{
	// 저장된 다음위치 얻어오기
	Vec3 NextPos = m_vNextPos;

	// 다음 지점이 없는경우 false 반환
	if (isnan(NextPos.x))
		return false;

	// 현재 위치와 방향 구하기
	Vec3 curPos = GetOwner()->Transform()->GetRelativePos();				// 현재 위치
	Vec3 vFront = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::FRONT); // Front 방향벡터
	Vec3 curDir = GetOwner()->Transform()->GetRelativeRot();				// 현재 방향
	Vec3 Dir = (NextPos - curPos).Normalize();								// 목적지 방향


	// 방향전환
	// 1. 현재 방향 얻어오기
	// 2. 목적 방향 얻어오기
	// 3. if(현재방향 - 목적지방향 > 오차범위) 현재방향 오차범위만큼 돌려준다.
	// 4. 넘지 않앗다면 목적지 방향으로 세팅.

	float DestDir = GetFrontDir(Dir);

	// Change Direction
	if (DIR_ERROR_RANGE < fabsf(m_fDestDir - curDir.y))
		DestDir = isLeft(Dir, vFront) ? curDir.y + DIR_ADD_RADIAN : curDir.y - DIR_ADD_RADIAN;
	else
		DestDir = m_fDestDir;

	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, DestDir, 0.f));

	float speed = _fSpeed;
	Vec3 newPos = curPos + (speed * Dir * DT);
	GetOwner()->Transform()->SetRelativePos(newPos);

	if ((newPos - NextPos).Length() < _fSpeed * DT)
	{
		FindNextPath();
	}

	return true;
}

void CFindPath::ClearPath()
{
	m_vNextPos = Vec3(NaN, NaN, NaN);
}

void CFindPath::SaveToLevelFile(FILE* _File)
{
}

void CFindPath::LoadFromLevelFile(FILE* _FILE)
{
}

float CFindPath::GetFrontDir(Vec3 _Direction)
{
	// Cal Front Dir
	float yRad = atan2(-DirectX::XMVectorGetX(_Direction),
		sqrt(DirectX::XMVectorGetY(_Direction) *
			DirectX::XMVectorGetY(_Direction) +
			DirectX::XMVectorGetZ(_Direction) *
			DirectX::XMVectorGetZ(_Direction)));

	// 위를 향하는 경우 radian 구해주기
	if (_Direction.z > 0.0f)
		yRad = (DirectX::XM_PI - yRad);

	return yRad;
}

bool CFindPath::isLeft(const Vec3& _objDir, const Vec3& _DestDir)
{
	Vec3 yAxis = { 0.0f, 1.0f, 0.0f };		// y축 벡터
	Vec3 corss = _objDir.Cross(_DestDir);	// 두 벡터의 외적

	// 외적 결과와 y축 벡터의 내적을 계산하여 부호 판단
	float dot = corss.Dot(yAxis);

	return dot > 0.0f;	// 양수면 왼쪽, 음수면 오른쪽
}
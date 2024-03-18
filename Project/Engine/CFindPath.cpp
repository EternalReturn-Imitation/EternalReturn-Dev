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

#define DIR_ERROR_RANGE 0.0872665f	// ��������	5�� (radian)
#define DIR_ADD_RADIAN	0.0872665f	// ��ȭ ���� 5�� (radian)
#define RADIAN360		6.23819f	// 360���� �ش��ϴ� radian


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
	// 경로 초기화
	m_vecPath.clear();
	
	// 현재 위치를 받아온다
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

	float dist = Vec3::Distance(CurPos, endPos);
	if (dist < 1.f)
		false;


	// 현재 위치에서 목표 위치로의 경로를 검색한다
	m_vecPath = CPathFindMgr::GetInst()->FindPath(CurPos, endPos);
	m_iPathCount = (UINT)m_vecPath.size();
	m_iCurPathIdx = 0;

	// 경로가 없거나 목표 지점과 현재 지점이 같은 경우
	if (m_iPathCount <= 1)
	{
		// 경로 초기화 및 이동 정보 설정
		m_vecPath.clear();
		m_vNextPos = Vec3(NaN, NaN, NaN);	// 다음 위치를 무효값으로 설정
		m_vPrevEndPos = endPos;
		return false; // 경로 탐색 실패
	}

	// 다음 이동할 위치 설정
	m_vNextPos = Vec3(m_vecPath[1].x, m_vecPath[1].y, m_vecPath[1].z);

	// 이동 방향 계산
	Vec3 Dir = (m_vNextPos - CurPos).Normalize();
	m_fDestDir = GetFrontDir(Dir); // 정면 방향 갱신

	m_vPrevEndPos = endPos;

	return true;
}

bool CFindPath::FindNextPath()
{
	m_iCurPathIdx++;

	// 다음 경로 인덱스가 경로 배열의 범위 내에 있으면
	if (m_iCurPathIdx != -1 && m_iCurPathIdx < m_iPathCount)
	{
		// 다음 이동할 위치 설정
		m_vNextPos = Vec3(m_vecPath[m_iCurPathIdx].x, m_vecPath[m_iCurPathIdx].y, m_vecPath[m_iCurPathIdx].z);

		// 이동방향 계산
		Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
		Vec3 vNextDir = (m_vNextPos - CurPos).Normalize();	// 다음 방향 벡터
		m_fDestDir = GetFrontDir(vNextDir);	// 다음 목표 방향 설정

		return true;
	}
	// 다음 경로 인덱스가 범위를 벗어나면
	else
	{
		// 다음 위치를 무효값으로 설정하고 이동방향 초기화
		m_vNextPos = Vec3(NaN, NaN, NaN);
		m_fDestDir = 0.f;

		return false;
	}
}

bool CFindPath::PathMove(float _fSpeed)
{
	// 다음 이동 위치 가져오기
	Vec3 NextPos = m_vNextPos;

	// 유효한 값이 아니면 이동 종료
	if (isnan(NextPos.x))
		return false;

	// 현재 위치와 방향 가져오기
	Vec3 curPos = GetOwner()->Transform()->GetRelativePos();				// 현재 위치
	Vec3 vFront = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::FRONT); // Front 방향 벡터
	Vec3 curDir = GetOwner()->Transform()->GetRelativeRot();				// 현재 방향
	Vec3 Dir = (NextPos - curPos).Normalize();								// 다음 방향 벡터
	// 방향 변경
	float DestDir = GetFrontDir(Dir);

	// 방향 오차 계산
	float CheckErrorRange = fabsf(m_fDestDir - curDir.y);

	if (RADIAN360 < CheckErrorRange)
		CheckErrorRange -= RADIAN360;

	if (DIR_ERROR_RANGE < CheckErrorRange)
		DestDir = IsLeft(Dir, vFront) ? curDir.y + DIR_ADD_RADIAN : curDir.y - DIR_ADD_RADIAN;
	else
		DestDir = m_fDestDir;

	// 방향 설정
	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, DestDir, 0.f));

	// 이동 속도 설정
	float speed = _fSpeed;
	Vec3 newPos = curPos + (speed * Dir * DT);
	GetOwner()->Transform()->SetRelativePos(newPos);

	// 다음 위치까지의 거리가 이동 속도보다 짧으면 다음 경로로 이동
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
	// 앞쪽 방향 계산
	float yRad = atan2(-DirectX::XMVectorGetX(_Direction),
		sqrt(DirectX::XMVectorGetY(_Direction) *
			DirectX::XMVectorGetY(_Direction) +
			DirectX::XMVectorGetZ(_Direction) *
			DirectX::XMVectorGetZ(_Direction)));

	// 벡터가 양수인 경우 180도 회전
	if (_Direction.z > 0.0f)
		yRad = (DirectX::XM_PI - yRad);

	return yRad;
}

bool CFindPath::IsLeft(const Vec3& _objDir, const Vec3& _DestDir)
{
	Vec3 yAxis = { 0.0f, 1.0f, 0.0f };		// y축 벡터
	Vec3 corss = _objDir.Cross(_DestDir);	// 두 벡터의 외적 계산

	// 외적 결과와 y축 벡터의 내적 계산하여 왼쪽 여부 판단
	float dot = corss.Dot(yAxis);

	return dot > 0.0f;	// 양수면 왼쪽, 음수면 오른쪽
}

Vec3 CFindPath::findMaxClearPoint(const Vec3& _dir, float _min, float _max)
{
	float left = _min;
	float right = _max;
	float maxClearDistance = 0;

	Vec3 vOwnerPos = GetOwner()->Transform()->GetRelativePos();

	// while (left <= maxClearDistance)
	// {
	// 	Vec3 tmp(_dir.x * maxClearDistance, _dir.y * maxClearDistance, _dir.z * maxClearDistance);
	// 
	// 	if (CPathFindMgr::GetInst()->IsValidPoint(tmp))
	// 	{
	// 		break;
	// 	}
	// 	else
	// 	{
	// 		maxClearDistance -= 0.5;
	// 	}
	// }

	// binary

	while (left <= right)
	{
		float mid = (left + right) / 2.f;

		Vec3 tmp(_dir.x * mid, _dir.y * mid, _dir.z * mid);
		Vec3 CheckPos = vOwnerPos + tmp;

		if (CPathFindMgr::GetInst()->IsValidPoint(CheckPos))
		{
			maxClearDistance = mid;
			left = mid + 0.1f;
		}
		else
		{
			right = mid - 0.1f;
		}
	}

	Vec3 res(_dir.x * maxClearDistance, _dir.y * maxClearDistance, _dir.z * maxClearDistance);

	return res;
}

float CFindPath::findMaxClearDistance(const Vec3& _dir, float _min, float _max)
{
	float left = _min;
	float right = _max;
	float maxClearDistance = 0.f;

	Vec3 vOwnerPos = GetOwner()->Transform()->GetRelativePos();
	
	// binary

	while (left <= right)
	{
		float mid = (left + right) / 2.f;

		Vec3 tmp(_dir.x * mid, _dir.y * mid, _dir.z * mid);
		Vec3 CheckPos = vOwnerPos + tmp;

		if (CPathFindMgr::GetInst()->IsValidPoint(CheckPos))
		{
			maxClearDistance = mid;
			left = mid + 0.1f;
		}
		else
		{
			right = mid - 0.1f;
		}
	}

	return maxClearDistance;
}







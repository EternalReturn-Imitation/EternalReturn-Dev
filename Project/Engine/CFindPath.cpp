#include "pch.h"
#include "CFindPath.h"

#include "CNaviMap.h"
#include "CLevelMgr.h"
#include "CGameObject.h"
#include "CKeyMgr.h"
#include "CPathFindMgr.h"
#include "CTransform.h"
#include "CTimeMgr.h"

CFindPath::CFindPath()
	:CComponent(COMPONENT_TYPE::FINDPATH)
	, LaycastResultTrigger(false)
	, m_vNextPos(Vec3(NaN, NaN, NaN))
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
	//if (KEY_PRESSED(KEY::LBTN))
	//{
	//	LaycastResultTrigger = true;
	//
	//	tNaviResult naviResult = CPathFindMgr::GetInst()->GetNaviResult();
	//	float yPos = CPathFindMgr::GetInst()->FindYPos(Vec2(naviResult.resultPos.x, naviResult.resultPos.z));
	//	naviResult.resultPos.y = yPos;
	//	Vec3 curPos = GetOwner()->Transform()->GetRelativePos();
	//	GetOwner()->Transform()->SetRelativePos(Vec3(naviResult.resultPos.x,naviResult.resultPos.y,naviResult.resultPos.z));
	//	
	//	int a = 0;
	//}
	//
	////여기에 naviResult변수를 만들면 한 틱 느리게 값을 받음.
	//if (LaycastResultTrigger) {
	//
	//	LaycastResultTrigger = false;
	//}

	if (KEY_PRESSED(KEY::LBTN)) {
		tNaviResult naviResult = CPathFindMgr::GetInst()->GetNaviResult();
		//GetOwner()->Transform()->SetRelativePos(Vec3(naviResult.resultPos.x,naviResult.resultPos.y,naviResult.resultPos.z));
		FindPath(Vec3(naviResult.resultPos.x, naviResult.resultPos.y, naviResult.resultPos.z));
	}
	PathMove(50.0f, false);
}

void CFindPath::FindPath(Vec3 endPos)
{
	// 새 경로를 찾을 때마다 새로운 경로로 대체함
	float dist = (endPos - m_vPrevEndPos).Length();

	//if (dist > 10.f) //이전에 찍어둔 목적지와 현재 요청한 목적지의 차이가 10픽셀밖에안된다면 길찾기를 진행하지 않음 : 매번 비슷한곳을찍었을때 캐릭터가 덜덜떠는 현상 방지하기 위함!
	//{
	m_vecPath.clear();
	// 오브젝트의 현재 위치
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

	// 현재 경로를 시작으로 경로를 받아옴
	m_vecPath = CPathFindMgr::GetInst()->FindPath(CurPos, endPos);
	m_iPathCount = m_vecPath.size();
	m_iCurPathIdx = 0;

	m_vNextPos = Vec3(m_vecPath[0].x, m_vecPath[0].y, m_vecPath[0].z);

	m_vPrevEndPos = endPos;
	//}
}

void CFindPath::FindNextPath()
{
	m_iCurPathIdx++;

	// 다음 경로가 있다면 그 위치를 반환하고, 경로 인덱스 증가시킴
	if (m_iCurPathIdx != -1 && m_iCurPathIdx < m_iPathCount)
	{
		m_vNextPos = Vec3(m_vecPath[m_iCurPathIdx].x, m_vecPath[m_iCurPathIdx].y, m_vecPath[m_iCurPathIdx].z);
	}
	// 다음 경로가 없다면 갈 수 없는 위치 반환
	else
	{
		m_vNextPos = Vec3(NaN, NaN, NaN);
	}
}

bool CFindPath::PathMove(float _fSpeed, bool _IsRotation)
{
	Vec3 NextPos = m_vNextPos;

	if (isnan(NextPos.x))
		return true;

	Vec3 curPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 Dir = (NextPos - curPos).Normalize();

	float speed = _fSpeed;

	Vec3 newPos = curPos + (speed* Dir * DT);

	GetOwner()->Transform()->SetRelativePos(newPos);

	if ((newPos - NextPos).Length() < _fSpeed * DT) {
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

void CFindPath::SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType)
{
}

void CFindPath::LoadFromDB(int _gameObjectID)
{
}

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
	// �Ÿ��Ǵܿ�
	// float dist = (endPos - m_vPrevEndPos).Length();

	// ������ ��� �ʱ�ȭ
	m_vecPath.clear();
	
	// ������Ʈ�� ���� ��ġ
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

	// ���� ��θ� �������� ��θ� �޾ƿ�
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

	// ������ ���� ����
	Vec3 Dir = (m_vNextPos - CurPos).Normalize();
	m_fDestDir = GetFrontDir(Dir);

	m_vPrevEndPos = endPos;

	return true;
}

bool CFindPath::FindNextPath()
{
	m_iCurPathIdx++;

	// ���� ��ΰ� �ִٸ� �� ��ġ�� ��ȯ�ϰ�, ��� �ε��� ������Ŵ
	if (m_iCurPathIdx != -1 && m_iCurPathIdx < m_iPathCount)
	{
		m_vNextPos = Vec3(m_vecPath[m_iCurPathIdx].x, m_vecPath[m_iCurPathIdx].y, m_vecPath[m_iCurPathIdx].z);

		// �����̵���ΰ� �ִ�.
		Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
		Vec3 vNextDir = (m_vNextPos - CurPos).Normalize();	// ���ο� ������ ����
		m_fDestDir = GetFrontDir(vNextDir);

		return true;
	}
	// ���� ��ΰ� ���ٸ� �� �� ���� ��ġ ��ȯ
	else
	{
		m_vNextPos = Vec3(NaN, NaN, NaN);
		m_fDestDir = 0.f;

		return false;
	}
}

bool CFindPath::PathMove(float _fSpeed)
{
	// ����� ������ġ ������
	Vec3 NextPos = m_vNextPos;

	// ���� ������ ���°�� false ��ȯ
	if (isnan(NextPos.x))
		return false;

	// ���� ��ġ�� ���� ���ϱ�
	Vec3 curPos = GetOwner()->Transform()->GetRelativePos();				// ���� ��ġ
	Vec3 vFront = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::FRONT); // Front ���⺤��
	Vec3 curDir = GetOwner()->Transform()->GetRelativeRot();				// ���� ����
	Vec3 Dir = (NextPos - curPos).Normalize();								// ������ ����


	// ������ȯ
	// 1. ���� ���� ������
	// 2. ���� ���� ������
	// 3. if(������� - ���������� > ��������) ������� ����������ŭ �����ش�.
	// 4. ���� �ʾѴٸ� ������ �������� ����.

	float DestDir = GetFrontDir(Dir);

	// Change Direction

	float CheckErrorRange = fabsf(m_fDestDir - curDir.y);
	
	if (RADIAN360 < CheckErrorRange)
		CheckErrorRange -= RADIAN360;

	if (DIR_ERROR_RANGE < CheckErrorRange)
		DestDir = isLeft(Dir, vFront) ? curDir.y + DIR_ADD_RADIAN : curDir.y - DIR_ADD_RADIAN;
	else
		DestDir = m_fDestDir;

	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, DestDir, 0.f));

	// ��η� ������Ʈ �̵�
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

	// ���� ���ϴ� ��� radian �����ֱ�
	if (_Direction.z > 0.0f)
		yRad = (DirectX::XM_PI - yRad);

	return yRad;
}

bool CFindPath::isLeft(const Vec3& _objDir, const Vec3& _DestDir)
{
	Vec3 yAxis = { 0.0f, 1.0f, 0.0f };		// y�� ����
	Vec3 corss = _objDir.Cross(_DestDir);	// �� ������ ����

	// ���� ����� y�� ������ ������ ����Ͽ� ��ȣ �Ǵ�
	float dot = corss.Dot(yAxis);

	return dot > 0.0f;	// ����� ����, ������ ������
}
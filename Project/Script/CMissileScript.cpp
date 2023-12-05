#include "pch.h"
#include "CMissileScript.h"

CMissileScript::CMissileScript()
	: CScript((UINT)SCRIPT_TYPE::MISSILESCRIPT)
	, m_vDir(Vec3(0.f, 1.f, 0.f))
	, m_fSpeed(100.f)
	, m_TargetObject(nullptr)
{
}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::begin()
{
	// 레벨에 존재하는 몬스터를 찾아서 타겟으로 삼는다.
	m_TargetObject = CLevelMgr::GetInst()->FindObjectByName(L"Monster");
}

void CMissileScript::tick()
{
	// 미사일 본인의 위치값을 알아낸다.
	Vec3 vPos = Transform()->GetRelativePos();

	if(IsValidObj(m_TargetObject))
	{
		Vec3 vTargetPos = m_TargetObject->Transform()->GetWorldPos();

		// 미사일에서 타겟을 향한 방향을 알아낸다.
		Vec3 vDir = vTargetPos - vPos;
		vDir.z = 0.f;
		vDir.Normalize();
		
		// 타겟을 향한 방향으로 m_fSpeed 속도로 이동한다.
		vPos += vDir* m_fSpeed* DT;
		Transform()->SetRelativePos(vPos);
	}
	else
	{
		vPos += m_vDir * DT * m_fSpeed;
		Transform()->SetRelativePos(vPos);
	}

	//int a = 1;
	//MeshRender()->GetDynamicMaterial()->SetScalarParam(INT_2, &a);
}

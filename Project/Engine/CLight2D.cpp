#include "pch.h"
#include "CLight2D.h"

#include "CRenderMgr.h"
#include "CTransform.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

CLight2D::CLight2D()
	: CComponent(COMPONENT_TYPE::LIGHT2D)
{
}

CLight2D::~CLight2D()
{
}

void CLight2D::finaltick()
{
	if (KEY_PRESSED(KEY::C))
	{
		Vec3 vRot = Transform()->GetRelativeRot();
		vRot.z += DT * (XM_PI / 2.f);
		Transform()->SetRelativeRot(vRot);
	}

	m_LightInfo.vWorldPos = Transform()->GetWorldPos();
	m_LightInfo.vWorldDir = Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	CRenderMgr::GetInst()->RegisterLight2D(m_LightInfo);
}

void CLight2D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_LightInfo, sizeof(tLightInfo), 1, _File);
}

void CLight2D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_LightInfo, sizeof(tLightInfo), 1, _File);
}

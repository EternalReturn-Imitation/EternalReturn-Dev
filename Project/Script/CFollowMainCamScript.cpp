#include "pch.h"
#include "CFollowMainCamScript.h"

#include <Engine\CRenderMgr.h>


CFollowMainCamScript::CFollowMainCamScript()
	: CScript((UINT)SCRIPT_TYPE::FOLLOWMAINCAMSCRIPT)
	, pTargetObject(nullptr)
	, OffsetPos{}
{
}

CFollowMainCamScript::~CFollowMainCamScript()
{
}

void CFollowMainCamScript::begin()
{
	CCamera* camera = CRenderMgr::GetInst()->GetMainCam();
	pTargetObject = CRenderMgr::GetInst()->GetMainCam()->GetOwner();
}

void CFollowMainCamScript::tick()
{
	if (pTargetObject)
	{
		Vec3 TargetPos = pTargetObject->Transform()->GetRelativePos();
		Vec3 vPos = TargetPos + OffsetPos;
		
		Transform()->SetRelativePos(vPos);
	}
}

void CFollowMainCamScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&OffsetPos, sizeof(Vec3), 1, _File);
}

void CFollowMainCamScript::LoadFromLevelFile(FILE* _File)
{
	fread(&OffsetPos, sizeof(Vec3), 1, _File);
}

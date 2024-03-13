#include "pch.h"
#include "CAnim3D.h"

#include "CAnimator3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"
#include "CBone.h"

#include "CAnimation3DShader.h"

#include "CKeyMgr.h"

CAnim3D* CAnim3D::CreateAnimation(Ptr<CBone> _pBone)
{
	CAnim3D* pAnim = new CAnim3D();
	pAnim->m_pBone = _pBone;
	pAnim->m_tMTAnimClip = pAnim->m_pBone->GetAnimClip()->at(0);
	pAnim->m_tMTAnimClip.fUpdateTime = 0.f;

	return pAnim;
}

CAnim3D::CAnim3D()
	: m_pOwner(nullptr)
	, m_tMTAnimClip{}
{
}

CAnim3D::CAnim3D(const CAnim3D& _origin)
	: m_pOwner(nullptr)
	, m_tMTAnimClip(_origin.m_tMTAnimClip)
	, m_pBone(_origin.m_pBone)
{
}

CAnim3D::~CAnim3D()
{
}
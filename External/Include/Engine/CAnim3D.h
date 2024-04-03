#pragma once
#include "CEntity.h"
#include "CBone.h"
#include "ptr.h"

class CStructuredBuffer;

class CAnimator3D;

class CAnim3D :
    public CEntity
{
private:
    CAnimator3D*    m_pOwner;
	
	Ptr<CBone>		m_pBone;	// 애니메이션 본

	tMTAnimClip		m_tMTAnimClip;

public:
	Ptr<CBone> GetBone() { return m_pBone; }

	void SetAnimClip(tMTAnimClip& _tMTAnimClip) { m_tMTAnimClip = _tMTAnimClip; }

	tMTAnimClip& GetAnimClip() { return m_tMTAnimClip; }

	void Reset() { m_tMTAnimClip.fUpdateTime = 0.f; }

	void SetOwner(CAnimator3D* _pOwner) { m_pOwner = _pOwner; }
	static CAnim3D* CreateAnimation(Ptr<CBone> _pBone);

	CLONE(CAnim3D);

public:
	CAnim3D();
	CAnim3D(const CAnim3D& _origin);
	~CAnim3D();

	friend class CAnimator3D;
};
#pragma once
#include "CEntity.h"

#include "ptr.h"

class CAnimator3D;

class CAnim3D :
    public CEntity
{
private:
    CAnimator3D*    m_pOwner;
	
	double			m_dCurTime;		// 현재시간

	tMTAnimClip		m_tMTAnimClip;

public:
	void finaltick();

	tMTAnimClip& GetAnimClip() { return m_tMTAnimClip; }
	double GetAnimTime() { return m_dCurTime; }

	void SetOwner(CAnimator3D* _pOwner) { m_pOwner = _pOwner; }
	void Create(const tMTAnimClip& _tMTAnimClip) 
	{
		m_tMTAnimClip = _tMTAnimClip; 
		m_tMTAnimClip.fUpdateTime = 0.f;
		m_dCurTime = 0.f;
	}

	CLONE(CAnim3D);

public:
	CAnim3D();
	CAnim3D(const CAnim3D& _origin);
	~CAnim3D();
};


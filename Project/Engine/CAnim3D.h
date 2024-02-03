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

	vector<Matrix>	m_vecFinalBoneMat;		// 텍스쳐에 전달할 최종 행렬정보
	int				m_iFrameCount;			// 30

	int				m_iFrameIdx;			// 클립의 현재 프레임
	int				m_iNextFrameIdx;		// 클립의 다음 프레임
	float			m_fRatio;				// 프레임 사이 비율

	// Anim
	double			m_dCurTime;		// 현재시간

	tMTAnimClip		m_tMTAnimClip;

	bool			m_bPlay;		// 재생 여부

public:
	void finaltick();

	Ptr<CBone> GetBone() { return m_pBone; }

	tMTAnimClip& GetAnimClip() { return m_tMTAnimClip; }
	double GetAnimTime() { return m_dCurTime; }

	const int GetCurFrameIdx() { return m_iFrameIdx; }
	const int GetNextFrameIdx() { return m_iNextFrameIdx; }

	bool IsPlay() { return m_bPlay; }

	void Play() { m_bPlay = true; }
	void Stop() { m_bPlay = false; }
	void Reset() { m_tMTAnimClip.fUpdateTime = 0.f; }

	void SetFrame(int _Frame);

	void SetOwner(CAnimator3D* _pOwner) { m_pOwner = _pOwner; }
	static CAnim3D* CreateAnimation(Ptr<CBone> _pBone);

	CLONE(CAnim3D);

public:
	CAnim3D();
	CAnim3D(const CAnim3D& _origin);
	~CAnim3D();

	friend class CAnimator3D;
};


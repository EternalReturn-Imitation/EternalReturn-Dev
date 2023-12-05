#pragma once
#include "CEntity.h"

#include "ptr.h"
#include "CTexture.h"

class CAnimator2D;

class CAnim2D :
    public CEntity
{
private:
    CAnimator2D*        m_pOwner;
    vector<tAnim2DFrm>  m_vecFrm;
    Vec2                m_vBackSize;
    Ptr<CTexture>       m_AtlasTex;

    int                 m_iCurFrm;
    float               m_fTime;

    bool                m_bFinish;

public:
    void finaltick();
    void Create(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _BackSize, int _FrameCount, int _FPS);

    const tAnim2DFrm& GetCurFrame() { return m_vecFrm[m_iCurFrm];}
    Vec2 GetBackSize() { return m_vBackSize; }
    Ptr<CTexture> GetAtlasTex() { return m_AtlasTex; }

    bool IsFinish() { return m_bFinish; }
    void Reset()
    {
        m_iCurFrm = 0;
        m_fTime = 0.f;
        m_bFinish = false;
    }

    void SaveToLevelFile(FILE* _File);
    void LoadFromLevelFile(FILE* _File);


    CLONE(CAnim2D);
public:
    CAnim2D();
    ~CAnim2D();

    friend class CAnimator2D;
};


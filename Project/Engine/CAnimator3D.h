#pragma once
#include "CComponent.h"

#include "Ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CBone.h"

class CStructuredBuffer;

class CAnim3D;

class CAnimator3D :
    public CComponent
{
private:
    map<wstring, CAnim3D*>      m_mapAnim;
    CAnim3D*                    m_pCurAnim;

    int							m_iFrameCount;              // 30

    // default Animation
    int                         m_iFrameIdx;                // 현재 클립의 프레임
    int                         m_iNextFrameIdx;            // 클립의 다음 프레임
    float                       m_fRatio;                   // 프레임 사이 비율

    double                      m_dCurTime;                 // 현재 시간

    bool                        m_bPlay;                    // 재생 여부
    
    // Animation transitions Blending
    CAnim3D*                    m_pPreviousAnim;
    int                         m_iPreviousAnimFrmIdx;      // 이전 클립의 전환 당시 프레임
    bool                        m_bAnimTrans;               // 애니메이션 전환 여부

    double                      m_dTransitionUpdateTime;    // 애니메이션 진행 시간
    float                       m_fTransitionTime;          // 애니메이션 전환 시간
    float                       m_fTransitionsRatio;        // 애니메이션 전환 비율
     
    CStructuredBuffer*          m_pBoneFinalMatBuffer;      // 특정 프레임의 최종 행렬
    bool						m_bFinalMatUpdate;          // 최종행렬 연산 수행여부

public:
    virtual void finaltick() override;
    void UpdateData();

public:
    CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
    UINT GetBoneCount();
    void ClearData();

    map<wstring, CAnim3D*>& GetAnims() { return m_mapAnim; }
    CAnim3D* AddAnim(Ptr<CBone> _pBone);
    CAnim3D* GetPreviousAnim() { return m_pPreviousAnim; }
    CAnim3D* GetCurAnim() { return m_pCurAnim; }

    // int GetCurFrameIdx() { return m_iFrameIdx; }

public:
    void Play();
    void Stop();
    void Reset();
    void SetFrame(int _Frame);

    int GetCurFrame() { return m_iFrameIdx; }

    CAnim3D* SelectAnimation(const wstring& _AnimName);

    bool IsPlay();

private:
    void Check_Bone(Ptr<CBone> _pBone);

public:
    void SaveCurAnimDataToFile();

    virtual void SaveToLevelFile(FILE* _pFile) override;
    virtual void LoadFromLevelFile(FILE* _pFile) override;

    virtual void SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType) override;
    virtual void LoadFromDB(int _gameObjectID) override;

    CLONE(CAnimator3D);

public:
    CAnimator3D();
    CAnimator3D(const CAnimator3D& _origin);
    ~CAnimator3D();

    friend class CAnim3D;
};
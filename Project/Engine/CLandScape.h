#pragma once
#include "CRenderComponent.h"

#include "ptr.h"
#include "CHeightMapShader.h"
#include "CRaycastShader.h"
#include "CWeightMapShader.h"

enum class LANDSCAPE_MOD
{
    HEIGHT_MAP,
    SPLAT,
    NONE,
};

struct tWeight_4
{
    float arrWeight[4];
};

struct tWeight_8
{
    float arrWeight[8];
};


class CLandScape :
    public CRenderComponent
{
private:
    UINT                    m_iFaceX;
    UINT                    m_iFaceZ;

    Vec2                    m_vBrushScale;      // 브러쉬 크기(전체 지형대비 크기 비율값)
    Ptr<CTexture>           m_pBrushTex;        // 브러쉬용 텍스쳐

    Ptr<CRaycastShader>     m_pCSRaycast;       // 픽킹 쉐이더
    CStructuredBuffer*      m_pCrossBuffer;	    // 마우스 피킹되는 지점 정보 받는 버퍼

    Ptr<CHeightMapShader>   m_pCSHeightMap;     // 높이맵 쉐이더
    Ptr<CTexture>           m_HeightMap;        // 높이맵 텍스쳐

    Ptr<CWeightMapShader>   m_pCSWeightMap;     // 가중치 쉐이더
    CStructuredBuffer*      m_pWeightMapBuffer; // 가중치 저장 버퍼
    UINT					m_iWeightWidth;		// 가중치 버퍼 가로세로 행렬 수
    UINT					m_iWeightHeight;	// 가중치 버퍼 가로세로 행렬 수
    UINT                    m_iWeightIdx;		// 증가 시킬 가중치 부위

    LANDSCAPE_MOD           m_eMod; 	        // 지형 툴모드에서 상태값

    Ptr<CTexture>           m_pTileArrTex;      // 타일 배열 텍스쳐

public:
    void SetFace(UINT _iFaceX, UINT _iFaceZ);
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_HeightMap = _HeightMap; }

    virtual void finaltick() override;
    virtual void render() override;


private:
    void init();
    void CreateMesh();
    void CreateComputeShader();
    void CreateTexture();

    void Raycasting();

    CLONE(CLandScape);
public:
    CLandScape();
    ~CLandScape();
};


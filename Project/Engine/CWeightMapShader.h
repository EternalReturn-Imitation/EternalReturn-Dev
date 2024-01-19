#pragma once
#include "CComputeShader.h"

#include "ptr.h"
#include "CTexture.h"

class CStructuredBuffer;

class CWeightMapShader :
    public CComputeShader
{
private:
    CStructuredBuffer*  m_pWeightMap;
    CStructuredBuffer*  m_pRaycastInfo;

    Ptr<CTexture>       m_pBrushArrTex;
    Vec2                m_vBrushScale;
    int                 m_iBrushIdx;
    int                 m_iWeightIdx; // 가중치 수정할 위치

    int                 m_iWidth;
    int                 m_iHeight;

public:
    void SetWeightMap(CStructuredBuffer* _pWeightMap, UINT _iWidth, UINT _iHeight)
    {
        m_pWeightMap = _pWeightMap;
        m_iWidth = _iWidth;
        m_iHeight = _iHeight;
    }

    void SetInputBuffer(CStructuredBuffer* _pRaycastData)
    {
        m_pRaycastInfo = _pRaycastData;
    }

    void SetBrushArrTex(Ptr<CTexture> _pBrushTex)
    {
        m_pBrushArrTex = _pBrushTex;
    }

    void SetBrushScale(Vec2 _vScale)
    {
        m_vBrushScale = _vScale;
    }

    void SetBrushIndex(int _iIdx)
    {
        m_iBrushIdx = _iIdx;
    }

    void SetWeightIdx(int _iIdx)
    {
        m_iWeightIdx = _iIdx;
    }


public:
    virtual void UpdateData();
    virtual void Clear();

public:
    CWeightMapShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
    ~CWeightMapShader();
};
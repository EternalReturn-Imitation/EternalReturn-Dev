#pragma once
#include "CComputeShader.h"


#include "CTexture.h"

class CStructuredBuffer;

class CRaycastShader :
    public CComputeShader
{
private:
    // 지형의 높이 정보
    Ptr<CTexture>       m_pHeightMap;

    // 지형 면 개수
    UINT                m_iXFace;
    UINT                m_iZFace;

    // 카메라 Ray 정보
    tRay                m_ray;

    // 교점위치 출력 버퍼
    CStructuredBuffer* m_pOutput;

public:
    void SetFaceCount(UINT _x, UINT _z) { m_iXFace = _x; m_iZFace = _z; }
    void SetCameraRay(const tRay& _ray) { m_ray = _ray; }
    void SetOuputBuffer(CStructuredBuffer* _pOutputBuffer) { m_pOutput = _pOutputBuffer; }
    void SetHeightMap(Ptr<CTexture> _ptex) { m_pHeightMap = _ptex; }

protected:
    virtual void UpdateData();
    virtual void Clear();

public:
    CRaycastShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
    ~CRaycastShader();
};


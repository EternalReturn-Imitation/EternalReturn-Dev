#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CRaycastShader
	: public CComputeShader
{
private:
	UINT                m_iXFace;
	UINT                m_iZFace;

	// 카메라 Ray 정보
	tRay                m_ray;

	//검사할 삼각형 버퍼의 위치들
	vector<Vector4>			m_vVtx;

	//삼각형의 위치를 보내기 위한 구조화버퍼
	CStructuredBuffer* m_pVtx;

	// 교점위치 출력 버퍼
	CStructuredBuffer* m_pOutput;

	float m_xMin;
	float m_xMax;
	float m_yMin;
	float m_yMax;

public:
	void SetVtx(vector<Vector4> _vtx);
	void SetFaceCount(UINT _x, UINT _z) { m_iXFace = _x; m_iZFace = _z; }
	void SetCameraRay(const tRay& _ray) { m_ray = _ray; }
	void SetOuputBuffer(CStructuredBuffer* _pOutputBuffer) { m_pOutput = _pOutputBuffer; }

	void SetMinMax(float _xMin, float _xMax, float _yMin, float _yMax) {
		m_xMin = _xMin, m_xMax = _xMax, m_yMin = _yMin, m_yMax = _yMax;
	}

protected:

	virtual void UpdateData();
	virtual void Clear();

public:
	CRaycastShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
	~CRaycastShader();
};


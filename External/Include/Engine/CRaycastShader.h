#pragma once
#include "CComputeShader.h"

#include "ptr.h"
#include "CTexture.h"
#include "CRaycastShader.h"

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

	Vector3 m_vMaxScale;
	Vector3 m_vPos;

	Vector2 m_vXMinMax;
	Vector2 m_vYMinMax;
	Vector2 m_vZMinMax;

	//네비 맵 텍스트
	Ptr<CTexture>       m_pNaviTex;

public:
	void SetVtx(vector<Vector4> _vtx);
	void SetFaceCount(UINT _x, UINT _z) { m_iXFace = _x; m_iZFace = _z; }
	void SetCameraRay(const tRay& _ray) { m_ray = _ray; }
	void SetOuputBuffer(CStructuredBuffer* _pOutputBuffer) { m_pOutput = _pOutputBuffer; }
	void SetMaxScale(Vec3 _scale) { m_vMaxScale = _scale; }
	void SetNaviMapTex(Ptr<CTexture> _tex) { m_pNaviTex = _tex; }
	void SetPos(Vec3 _pos) { m_vPos = _pos; }
	void SetXMinMax(Vec2 _xMinMax) { m_vXMinMax = _xMinMax; }
	void SetYMinMax(Vec2 _yMinMax) { m_vYMinMax = _yMinMax; }
	void SetZMinMax(Vec2 _zMinMax) { m_vZMinMax = _zMinMax; }

	tRay GetRayInfo() { return m_ray; }
	vector<Vector4> GetVertex() { return m_vVtx; }

protected:

	virtual void UpdateData();
	virtual void Clear();

public:
	CRaycastShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
	~CRaycastShader();
};


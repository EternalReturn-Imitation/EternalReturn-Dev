#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CCopyBoneShader :
	public CComputeShader
{
private:
	CStructuredBuffer* m_pSrcBuffer;
	CStructuredBuffer* m_pDestBuffer;

public:
	// g_int_0 : BonCount
	void SetBoneCount(UINT _iBoneCount) { m_Const.arrInt[0] = _iBoneCount; }

	// g_int_1 : RowIndex	
	void SetRowIndex(UINT _iRowIdx) { m_Const.arrInt[1] = _iRowIdx; }

	void SetSourceBuffer(CStructuredBuffer* _pBuffer) { m_pSrcBuffer = _pBuffer; }
	void SetDestBuffer(CStructuredBuffer* _pBuffer) { m_pDestBuffer = _pBuffer; }

public:
	virtual void UpdateData();
	virtual void Clear();

public:
	CCopyBoneShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
	virtual ~CCopyBoneShader();
};


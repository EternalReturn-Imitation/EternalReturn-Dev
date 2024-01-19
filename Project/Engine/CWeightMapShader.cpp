#include "pch.h"
#include "CWeightMapShader.h"

#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

CWeightMapShader::CWeightMapShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
	: m_pWeightMap(nullptr)
	, m_pRaycastInfo(nullptr)
	, m_iBrushIdx(0)
	, m_iWeightIdx(0)
	, m_iWidth(0)
	, m_iHeight(0)
{
	m_iGroupPerThreadX = _iGroupPerThreadX;
	m_iGroupPerThreadY = _iGroupPerThreadY;
	m_iGroupPerThreadZ = _iGroupPerThreadZ;
}

CWeightMapShader::~CWeightMapShader()
{

}

void CWeightMapShader::UpdateData()
{
	m_Const.arrInt[0] = (int)m_iWidth;
	m_Const.arrInt[1] = (int)m_iHeight;
	m_Const.arrInt[2] = m_iBrushIdx;
	m_Const.arrInt[3] = m_iWeightIdx;
	m_Const.arrV2[0] = m_vBrushScale;

	m_pWeightMap->UpdateData_CS(0, false);

	m_pBrushArrTex->UpdateData_CS(0, true);

	m_pRaycastInfo->UpdateData_CS(16, true);

	// 그룹 수 계산
	m_iGroupX = (m_iWidth / m_iGroupPerThreadX) + 1;
	m_iGroupY = (m_iHeight / m_iGroupPerThreadY) + 1;
	m_iGroupZ = 1;
}

void CWeightMapShader::Clear()
{
	if (nullptr != m_pWeightMap)
	{
		m_pWeightMap->Clear_CS(false);
		m_pWeightMap = nullptr;
	}

	if (nullptr != m_pBrushArrTex)
	{
		m_pBrushArrTex->Clear(0);
		m_pBrushArrTex = nullptr;
	}

	if (nullptr != m_pRaycastInfo)
	{
		m_pRaycastInfo->Clear();
		m_pRaycastInfo = nullptr;
	}
}
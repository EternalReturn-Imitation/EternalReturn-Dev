#include "pch.h"
#include "CLandScapeRaycastShader.h"

#include "CConstBuffer.h"
#include "CStructuredBuffer.h"
#include "CTexture.h"

CLandScapeRaycastShader::CLandScapeRaycastShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
	: m_iXFace(0)
	, m_iZFace(0)
	, m_ray{}
	, m_pOutput(nullptr)
	, m_pHeightMap(nullptr)
{
	m_iGroupPerThreadX = _iGroupPerThreadX;
	m_iGroupPerThreadY = _iGroupPerThreadY;
	m_iGroupPerThreadZ = _iGroupPerThreadZ;
}

CLandScapeRaycastShader::~CLandScapeRaycastShader()
{
}

void CLandScapeRaycastShader::UpdateData()
{
	m_Const.arrInt[0] = m_iXFace;
	m_Const.arrInt[1] = m_iZFace;

	m_Const.arrV4[0] = m_ray.vStart;
	m_Const.arrV4[1] = m_ray.vDir;

	// 높이맵 (SRV)
	m_pHeightMap->UpdateData_CS(0, true);

	// 출력 버퍼	(UAV)
	m_pOutput->UpdateData_CS(0, false);

	// 그룹 수 계산
	m_iGroupX = ((m_iXFace * 2) / m_iGroupPerThreadX) + 1;
	m_iGroupY = (m_iZFace / m_iGroupPerThreadY) + 1;
	m_iGroupZ = 1;
}

void CLandScapeRaycastShader::Clear()
{
	m_pHeightMap->Clear_CS(0);
	m_pOutput->Clear();
}
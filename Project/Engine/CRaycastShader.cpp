#include "pch.h"
#include "CRaycastShader.h"

#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

CRaycastShader::CRaycastShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
	: m_iXFace(128)
	, m_iZFace(128)
	, m_ray{}
	, m_pOutput(nullptr)
{
	m_iGroupPerThreadX = _iGroupPerThreadX;
	m_iGroupPerThreadY = _iGroupPerThreadY;
	m_iGroupPerThreadZ = _iGroupPerThreadZ;

	m_pVtx = new CStructuredBuffer;
}

CRaycastShader::~CRaycastShader()
{
	if (nullptr != m_pVtx)
		delete m_pVtx;
}

void CRaycastShader::SetVtx(vector<Vector4> _vtx)
{
	m_vVtx = _vtx;

	m_pVtx->Create(sizeof(Vector4), m_vVtx.size(), SB_TYPE::READ_ONLY, false, m_vVtx.data());
}

void CRaycastShader::UpdateData()
{
	m_Const.arrInt[0] = 220;
	m_Const.arrInt[1] = 193;
	m_Const.arrInt[2] = m_vVtx.size();

	m_Const.arrV4[0] = m_ray.vStart;
	m_Const.arrV4[1] = m_ray.vDir;

	m_Const.arrFloat[0] = m_xMin;
	m_Const.arrFloat[1] = m_xMax;
	m_Const.arrFloat[2] = m_yMin;
	m_Const.arrFloat[3] = m_yMax;

	// 출력 버퍼	(UAV)
	m_pOutput->UpdateData_CS(0, false);

	// 그룹 수 계산
	m_iGroupX = ((m_iXFace * 2) / m_iGroupPerThreadX) + 1;
	m_iGroupY = (m_iZFace / m_iGroupPerThreadY) + 1;
	m_iGroupZ = 1;

	//인풋 버퍼(t20)
	m_pVtx->UpdateData_CS(25, true);
}

void CRaycastShader::Clear()
{
	m_pOutput->Clear();
	m_pVtx->Clear();
}
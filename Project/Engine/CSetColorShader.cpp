#include "pch.h"
#include "CSetColorShader.h"

#include "CTexture.h"

CSetColorShader::CSetColorShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
{
	m_iGroupPerThreadX = _iGroupPerThreadX;
	m_iGroupPerThreadY = _iGroupPerThreadY;
	m_iGroupPerThreadZ = _iGroupPerThreadZ;
}

CSetColorShader::~CSetColorShader()
{
}

void CSetColorShader::SetTargetTexture(Ptr<CTexture> _Tex)
{
	m_OutTex = _Tex;

	if (nullptr != m_OutTex)
	{
		Vec2 vResolution = Vec2(m_OutTex->Width(), m_OutTex->Height());
		m_Const.arrV2[0] = vResolution;
	}
}

void CSetColorShader::UpdateData()
{
	m_OutTex->UpdateData_CS(0, false);

	// 그룹 개수 계산
	m_iGroupX = (m_OutTex->Width() / m_iGroupPerThreadX) + 1;
	m_iGroupY = (m_OutTex->Height() / m_iGroupPerThreadY) + 1;
	m_iGroupZ = 1;
}

void CSetColorShader::Clear()
{
	m_OutTex->Clear_CS(false);
}

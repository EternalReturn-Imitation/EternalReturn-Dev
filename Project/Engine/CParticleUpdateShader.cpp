#include "pch.h"
#include "CParticleUpdateShader.h"

#include "CStructuredBuffer.h"

CParticleUpdateShader::CParticleUpdateShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
	: m_ParticleBuffer(nullptr)
	, m_ModuleData(nullptr)
	, m_RWBuffer(nullptr)
{
	m_iGroupPerThreadX = _iGroupPerThreadX;
	m_iGroupPerThreadY = _iGroupPerThreadY;
	m_iGroupPerThreadZ = _iGroupPerThreadZ;
}

CParticleUpdateShader::~CParticleUpdateShader()
{

}

void CParticleUpdateShader::SetParticleBuffer(CStructuredBuffer* _Buffer)
{
	m_ParticleBuffer = _Buffer;
	m_Const.arrInt[0] = m_ParticleBuffer->GetElementCount();
}

void CParticleUpdateShader::UpdateData()
{
	m_ParticleBuffer->UpdateData_CS(0, false);
	m_RWBuffer->UpdateData_CS(1, false);
	m_ModuleData->UpdateData_CS(20, true);
	m_NoiseTex->UpdateData_CS(21, true);

	// ±×·ì ¼ö
	m_iGroupX = (m_ParticleBuffer->GetElementCount() / m_iGroupPerThreadX) + 1;
}

void CParticleUpdateShader::Clear()
{
	m_ParticleBuffer->Clear_CS(false);
	m_RWBuffer->Clear_CS(false);
	m_ModuleData->Clear_CS(true);
	m_NoiseTex->Clear_CS(true);
}
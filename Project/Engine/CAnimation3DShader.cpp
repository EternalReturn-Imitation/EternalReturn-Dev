#include "pch.h"
#include "CAnimation3DShader.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

CAnimation3DShader::CAnimation3DShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
	: m_pFrameDataBuffer(nullptr)
	, m_pOffsetMatBuffer(nullptr)
	, m_pOutputBuffer(nullptr)
	, m_pPreviousFrameDataBuffer(nullptr)
{
	m_iGroupPerThreadX = _iGroupPerThreadX;
	m_iGroupPerThreadY = _iGroupPerThreadY;
	m_iGroupPerThreadZ = _iGroupPerThreadZ;
}

CAnimation3DShader::~CAnimation3DShader()
{
}

void CAnimation3DShader::UpdateData()
{
	// 구조화버퍼 전달
	m_pFrameDataBuffer->UpdateData_CS(16, true); // t16
	m_pOffsetMatBuffer->UpdateData_CS(17, true); // t17
	
	// 애니메이션 전환시
	if (m_Const.arrInt[3])
	{
		m_pPreviousFrameDataBuffer->UpdateData_CS(18, true); // t18
	}

	m_pOutputBuffer->UpdateData_CS(0, false);   // u0

	m_iGroupX = (m_Const.arrInt[0] / m_iGroupPerThreadX) + 1;
	m_iGroupY = 1;
	m_iGroupZ = 1;
}


void CAnimation3DShader::Clear()
{
	// 전달한 구조화버퍼 클리어	
	if (nullptr != m_pFrameDataBuffer)
	{
		m_pFrameDataBuffer->Clear_CS(true);
		m_pFrameDataBuffer = nullptr;
	}

	if (nullptr != m_pOffsetMatBuffer)
	{
		m_pOffsetMatBuffer->Clear_CS(true);
		m_pOffsetMatBuffer = nullptr;
	}

	if (nullptr != m_pOutputBuffer)
	{
		m_pOutputBuffer->Clear_CS(false);
		m_pOutputBuffer = nullptr;
	}

	if (nullptr != m_pPreviousFrameDataBuffer)
	{
		m_pPreviousFrameDataBuffer->Clear_CS(true);
		m_pPreviousFrameDataBuffer = nullptr;
	}
}
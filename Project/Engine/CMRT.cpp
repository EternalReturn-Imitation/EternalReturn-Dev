#include "pch.h"
#include "CMRT.h"

#include "CDevice.h"

CMRT::CMRT()
	: m_arrRT{}
	, m_RTCount(0)
	, m_ClearColor{}
	, m_Viewport{}
{
}

CMRT::~CMRT()
{
}

void CMRT::Create(Ptr<CTexture>* _arrRTTex, UINT _RTCount, Ptr<CTexture> _DSTex)
{
	for (UINT i = 0; i < _RTCount; ++i)
	{
		m_arrRT[i] = _arrRTTex[i];
	}

	m_RTCount = _RTCount;

	m_DSTex = _DSTex;

	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;

	m_Viewport.Width = _arrRTTex[0]->Width();
	m_Viewport.Height = _arrRTTex[0]->Height();

	m_Viewport.MinDepth = 0;
	m_Viewport.MaxDepth = 1;
}

void CMRT::ClearTarget()
{
	for (UINT i = 0; i < m_RTCount; ++i)
	{
		CONTEXT->ClearRenderTargetView(m_arrRT[i]->GetRTV().Get(), m_ClearColor[i]);
	}

	if (nullptr != m_DSTex)
	{
		CONTEXT->ClearDepthStencilView(m_DSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, (UINT)0.f);
	}	
}

void CMRT::OMSet(bool _bStay)
{
	ID3D11RenderTargetView* arrRTV[8] = {};
	for (UINT i = 0; i < m_RTCount; ++i)
	{
		arrRTV[i] = m_arrRT[i]->GetRTV().Get();
	}

	if (nullptr != m_DSTex)
	{
		CONTEXT->OMSetRenderTargets(m_RTCount, arrRTV, m_DSTex->GetDSV().Get());
	}
	else
	{
		ComPtr<ID3D11DepthStencilView> pDSV = nullptr;

		if (_bStay)
		{
			CONTEXT->OMGetRenderTargets(0, nullptr, pDSV.GetAddressOf());
		}

		CONTEXT->OMSetRenderTargets(m_RTCount, arrRTV, pDSV.Get());
	}

	CONTEXT->RSSetViewports(1, &m_Viewport);
}
#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"

#include "CStructuredBuffer.h"
#include "CMRT.h"

#include "CResMgr.h"
#include "CTexture.h"

void CRenderMgr::init()
{
    // Light2DBuffer 구조화 버퍼 생성
    m_Light2DBuffer = new CStructuredBuffer;
    m_Light2DBuffer->Create(sizeof(tLightInfo), 10, SB_TYPE::READ_ONLY, true);

    // Light3DBuffer 구조화 버퍼 생성
    m_Light3DBuffer = new CStructuredBuffer;
    m_Light3DBuffer->Create(sizeof(tLightInfo), 10, SB_TYPE::READ_ONLY, true);

    // MRT 생성
    CreateMRT();
}

void CRenderMgr::CreateMRT()
{
    // ====================
    // SwapChain MRT 만들기
    // ====================
    {
        m_MRT[(UINT)MRT_TYPE::SWAPCHAIN] = new CMRT;

        Ptr<CTexture> arrRTTex[8] = {};
        arrRTTex[0] = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");

        Ptr<CTexture> pDSTex = CResMgr::GetInst()->FindRes<CTexture>(L"DepthStencilTex");

        m_MRT[(UINT)MRT_TYPE::SWAPCHAIN]->Create(arrRTTex, 1, pDSTex);
        m_MRT[(UINT)MRT_TYPE::SWAPCHAIN]->SetClearColor(Vec4(0.3f, 0.3f, 0.3f, 1.f), 0);
    }

    // ====================
    // Deferred MRT 만들기
    // ====================
    {
        m_MRT[(UINT)MRT_TYPE::DEFERRED] = new CMRT;

        Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

        Ptr<CTexture> arrRTTex[8] = {};
        arrRTTex[0] = CResMgr::GetInst()->CreateTexture(L"ColorTargetTex", vResol.x, vResol.y
                                        , DXGI_FORMAT_R8G8B8A8_UNORM
                                        , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        arrRTTex[1] = CResMgr::GetInst()->CreateTexture(L"NormalTargetTex", vResol.x, vResol.y
                                        , DXGI_FORMAT_R32G32B32A32_FLOAT
                                        , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        arrRTTex[2] = CResMgr::GetInst()->CreateTexture(L"PositionTargetTex", vResol.x, vResol.y
                                        , DXGI_FORMAT_R32G32B32A32_FLOAT
                                        , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        arrRTTex[3] = CResMgr::GetInst()->CreateTexture(L"EmissiveTargetTex", vResol.x, vResol.y
                                        , DXGI_FORMAT_R8G8B8A8_UNORM
                                        , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        arrRTTex[4] = CResMgr::GetInst()->CreateTexture(L"DataTargetTex", vResol.x, vResol.y
                                        , DXGI_FORMAT_R32G32B32A32_FLOAT
                                        , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        m_MRT[(UINT)MRT_TYPE::DEFERRED]->Create(arrRTTex, 5, nullptr);
    }

    // ====================
    // Light MRT 만들기
    // ====================
    {
        m_MRT[(UINT)MRT_TYPE::DEFERRED_DECAL] = new CMRT;       

        Ptr<CTexture> arrRTTex[8] = {};
        arrRTTex[0] = CResMgr::GetInst()->FindRes<CTexture>(L"ColorTargetTex");
        arrRTTex[1] = CResMgr::GetInst()->FindRes<CTexture>(L"EmissiveTargetTex");

        m_MRT[(UINT)MRT_TYPE::DEFERRED_DECAL]->Create(arrRTTex, 2, nullptr);
    }



    // ====================
    // Light MRT 만들기
    // ====================
    {
        m_MRT[(UINT)MRT_TYPE::LIGHT] = new CMRT;

        Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

        Ptr<CTexture> arrRTTex[8] = {};
        arrRTTex[0] = CResMgr::GetInst()->CreateTexture(L"DiffuseTargetTex", vResol.x, vResol.y
                                , DXGI_FORMAT_R8G8B8A8_UNORM
                                , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        arrRTTex[1] = CResMgr::GetInst()->CreateTexture(L"SpecularTargetTex", vResol.x, vResol.y
                                , DXGI_FORMAT_R8G8B8A8_UNORM
                                , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        m_MRT[(UINT)MRT_TYPE::LIGHT]->Create(arrRTTex, 2, nullptr);
    }
}

void CRenderMgr::ClearMRT()
{
    for (UINT i = 0; i < (UINT)MRT_TYPE::END; ++i)
    {
        if (nullptr != m_MRT[i])
        {
            m_MRT[i]->ClearTarget();
        }         
    }
}
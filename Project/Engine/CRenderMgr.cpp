#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

#include "CCamera.h"
#include "CLight2D.h"
#include "CLight3D.h"

#include "CResMgr.h"
#include "CMRT.h"

CRenderMgr::CRenderMgr()
    : m_Light2DBuffer(nullptr)
    , m_Light3DBuffer(nullptr)
    , RENDER_FUNC(nullptr)
    , m_pEditorCam(nullptr)
    , m_MRT{}
{
    Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
    m_RTCopyTex = CResMgr::GetInst()->CreateTexture(L"RTCopyTex"
        , (UINT)vResolution.x, (UINT)vResolution.y
        , DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE
        , D3D11_USAGE_DEFAULT);

    CResMgr::GetInst()->FindRes<CMaterial>(L"GrayMtrl")->SetTexParam(TEX_0, m_RTCopyTex);

    CResMgr::GetInst()->FindRes<CMaterial>(L"DistortionMtrl")->SetTexParam(TEX_0, m_RTCopyTex);

}

CRenderMgr::~CRenderMgr()
{
    if (nullptr != m_Light2DBuffer)
        delete m_Light2DBuffer;

    if (nullptr != m_Light3DBuffer)
        delete m_Light3DBuffer;

    DeleteArray(m_MRT);
}



void CRenderMgr::render()
{
    // 광원 및 전역 데이터 업데이트 및 바인딩
    UpdateData();

    // MRT Clear    
    ClearMRT();

    // Dynamic ShadowMap
    render_shadowmap();

    // 렌더 함수 호출
    (this->*RENDER_FUNC)();

    // 광원 해제
    Clear();
}


void CRenderMgr::render_play()
{
    // 카메라 기준 렌더링
    for (size_t i = 0; i < m_vecCam.size(); ++i)
    {
        if (nullptr == m_vecCam[i])
            continue;

        // 물체 분류작업
        // - 해당 카메라가 볼 수 있는 물체(레이어 분류)
        // - 재질에 따른 분류 (재질->쉐이더) 쉐이더 도메인
        //   쉐이더 도메인에 따라서 렌더링 순서분류
        m_vecCam[i]->SortObject();

        m_MRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();

        m_vecCam[i]->render();
    }
}

void CRenderMgr::render_editor()
{
    // 물체 분류
    m_pEditorCam->SortObject();

    // 출력 타겟 지정    
    m_MRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();
    m_pEditorCam->render();
}


int CRenderMgr::RegisterCamera(CCamera* _Cam, int _idx)
{
    if (m_vecCam.size() <= _idx)
    {
        m_vecCam.resize(_idx + 1);
    }

    m_vecCam[_idx] = _Cam;
    return _idx;
}

void CRenderMgr::RegisterEditorCamera(CCamera* _Cam)
{
    m_pEditorCam = _Cam;
    m_pEditorCam->ViewDebugCube(false);
}

void CRenderMgr::SetRenderFunc(bool _IsPlay)
{
    if (_IsPlay)
        RENDER_FUNC = &CRenderMgr::render_play;
    else
        RENDER_FUNC = &CRenderMgr::render_editor;
}

void CRenderMgr::CopyRenderTarget()
{
    Ptr<CTexture> pRTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
    CONTEXT->CopyResource(m_RTCopyTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

void CRenderMgr::UpdateData()
{

    // 구조화버퍼의 크기가 모자라면 더 크게 새로 만든다.
    if (m_Light2DBuffer->GetElementCount() < (UINT)m_vecLight2DInfo.size())
    {
        m_Light2DBuffer->Create(sizeof(tLightInfo), (UINT)m_vecLight2DInfo.size(), SB_TYPE::READ_ONLY, true);
    }

    // 구조화버퍼로 광원 데이터를 옮긴다.
    m_Light2DBuffer->SetData(m_vecLight2DInfo.data(), sizeof(tLightInfo) * (UINT)m_vecLight2DInfo.size());

    m_Light2DBuffer->UpdateData(12, PIPELINE_STAGE::PS_PIXEL);

    if (m_Light3DBuffer->GetElementCount() < m_vecLight3DInfo.size())
    {
        m_Light3DBuffer->Create(sizeof(tLightInfo), (UINT)m_vecLight3DInfo.size(), SB_TYPE::READ_ONLY, true);
    }

    // 구조화버퍼로 광원 데이터를 옮긴다.
    m_Light3DBuffer->SetData(m_vecLight3DInfo.data(), sizeof(tLightInfo) * (UINT)m_vecLight3DInfo.size());
    m_Light3DBuffer->UpdateData(13, PIPELINE_STAGE::PS_PIXEL);


    // GlobalData 에 광원 개수정보 세팅
    GlobalData.Light2DCount = (UINT)m_vecLight2D.size();
    GlobalData.Light3DCount = (UINT)m_vecLight3D.size();

    // 전역 상수 데이터 바인딩
    CConstBuffer* pGlobalBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
    pGlobalBuffer->SetData(&GlobalData, sizeof(tGlobal));
    pGlobalBuffer->UpdateData();
    pGlobalBuffer->UpdateData_CS();
}

void CRenderMgr::render_shadowmap()
{
    // ShadowMap MRT 로 교체
    GetMRT(MRT_TYPE::SHADOWMAP)->OMSet();

    for (size_t i = 0; i < m_vecLight3D.size(); ++i)
    {
        m_vecLight3D[i]->render_shadowmap();
    }
}


void CRenderMgr::Clear()
{
    m_vecLight2D.clear();
    m_vecLight2DInfo.clear();
    m_vecLight3D.clear();
    m_vecLight3DInfo.clear();
}

#include "CLevelMgr.h"
#include "CLevel.h"
CCamera* CRenderMgr::GetMainCam()
{
    if (CLevelMgr::GetInst()->GetCurLevel()->GetState() == LEVEL_STATE::PLAY)
    {
        if (m_vecCam.empty())
            return nullptr;

        return m_vecCam[0];
    }
    else
    {
        return m_vecCam[0];
    }
}

CCamera* CRenderMgr::GetUICam()
{
    if (CLevelMgr::GetInst()->GetCurLevel()->GetState() == LEVEL_STATE::PLAY)
    {
        if (2 > m_vecCam.size())
            return nullptr;

        return m_vecCam[1];
    }
    else
        return nullptr;
}

#include "pch.h"
#include "CameraUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CCamera.h>

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>

#include <Engine\CTexture.h>

CameraUI::CameraUI()
    : ComponentUI("##Camera", COMPONENT_TYPE::CAMERA)
    , m_bLayerCheckWindow(false)
{
    SetName("Camera");
}

CameraUI::~CameraUI()
{
}

int CameraUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

    CCamera* pCamComponent = GetTarget()->Camera();

    float       fScale = pCamComponent->GetScale();             // Orthograpic 에서 사용하는 카메라 배율

    float       fNear = pCamComponent->GetNear();               // Near 값
    float       fFar = pCamComponent->GetFar();                 // Far 값

    float       fFOV = pCamComponent->GetFOV();                 // 화각
    bool        bMainCamera = pCamComponent->IsMainCamera();    // 메인카메라 여부

    float       fOrthoWidth = pCamComponent->GetOrthoWidth();   // OrthoGraphic 에서의 가로 투영 범위
    float       fOrthoHeight = pCamComponent->GetOrthoHeight(); // OrthoGraphic 에서의 세로 투영 범위

    PROJ_TYPE   eProjType = pCamComponent->GetProjType();       // 투영 타입

    int         iCamIdx= pCamComponent->GetCamIdx();            // 카메라 인덱스

    bool        bDebugView = pCamComponent->IsDebugView();      // 카메라 디버그큐브

    

    ImGui::Text("Distance     ");
    ImGui::SameLine();
    ImGui::DragFloatRange2("##CameraNearFar", &fNear, &fFar, 1.f, 1.f, 10000.f, "Near: %.f", "Far: %.f", ImGuiSliderFlags_AlwaysClamp);
    
    if (fNear == fFar)
        fFar += 1.f;

    ImGui::Text("FOV          ");
    ImGui::SameLine();
    ImGui::SliderAngle("##CameraFOV", &fFOV, 1, 179);

    ImGui::Text("LayerCheck   ");
    ImGui::SameLine();
    
    if (ImGui::Button("OpenWindow##LayerCheckWindow"))
        m_bLayerCheckWindow = true;
    
    ImGui::Text("FrustumLayer ");
    ImGui::SameLine();

    if (ImGui::Button("OpenWindow##FrustumCheckWindow"))
        m_bFrustumCheckWindow = true;

    if (m_bLayerCheckWindow)
        render_LayerCheck();
    
    if (m_bFrustumCheckWindow)
        render_FrustumCheck();


    fScale = pCamComponent->GetScale();             // Orthograpic 에서 사용하는 카메라 배율

    pCamComponent->SetNear(fNear);                  // Near 값
    pCamComponent->SetFar(fFar);                    // Far 값

    pCamComponent->SetFOV(fFOV);                    // 화각
    bMainCamera = pCamComponent->IsMainCamera();    // 메인카메라 여부

    pCamComponent->SetOrthoWidth(fOrthoWidth);      // OrthoGraphic 에서의 가로 투영 범위
    pCamComponent->SetOrthoHeight(fOrthoHeight);    // OrthoGraphic 에서의 세로 투영 범위

    pCamComponent->SetProjType(eProjType);          // 투영 타입

    pCamComponent->ViewDebugCube(bDebugView);       // 카메라 디버그큐브

    return TRUE;
}

void CameraUI::render_LayerCheck()
{
    CLevel*     CurLevel = CLevelMgr::GetInst()->GetCurLevel();
    UINT        iLayerMask = GetTarget()->Camera()->GetLayerMask();         // 촬영레이어 체크
    wstring     WLayerName;


    if (ImGui::Begin("Cam_LayerCheck", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking))
    {
        ImGui::BeginGroup();
        for (int i = 0; i < MAX_LAYER; ++i)
        {
            WLayerName = CurLevel->GetLayer(i)->GetName();

            // 레이어이름이 설정되어있지 않은 경우
            if (L"" == WLayerName)
                continue;

            string LayerNumb = "##Layer " + std::to_string(i);
            string LayerName;
            LayerName.assign(WLayerName.begin(), WLayerName.end());

            bool bChecked = iLayerMask & (1 << i);
            
            ImGui::Button(LayerName.c_str());
            ImGui::SameLine();
            ImGui::Checkbox(LayerNumb.c_str(), &bChecked);

            if(bChecked)
                iLayerMask |= 1 << i;
            else
                iLayerMask &= ~(1 << i);
        }
        ImGui::EndGroup();

        if (ImGui::Button("Close##LayerCheckWinCloseBtn"))
        {
            m_bLayerCheckWindow = false;
            ImGui::CloseCurrentPopup();
        };

        GetTarget()->Camera()->SetLayerMask(iLayerMask);
        
        ImGui::End();
    }
}

void CameraUI::render_FrustumCheck()
{
    CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
    UINT        iFrustumLayer = GetTarget()->Camera()->GetLayerFrustum();         // 촬영레이어 체크
    wstring     WLayerName;


    if (ImGui::Begin("Cam_FrustumCheck", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking))
    {
        ImGui::BeginGroup();
        for (int i = 0; i < MAX_LAYER; ++i)
        {
            WLayerName = CurLevel->GetLayer(i)->GetName();

            // 레이어이름이 설정되어있지 않은 경우
            if (L"" == WLayerName)
                continue;

            string LayerNumb = "##Layer " + std::to_string(i);
            string LayerName;
            LayerName.assign(WLayerName.begin(), WLayerName.end());

            bool bChecked = iFrustumLayer & (1 << i);

            ImGui::Button(LayerName.c_str());
            ImGui::SameLine();
            ImGui::Checkbox(LayerNumb.c_str(), &bChecked);

            if (bChecked)
                iFrustumLayer |= 1 << i;
            else
                iFrustumLayer &= ~(1 << i);
        }
        ImGui::EndGroup();

        if (ImGui::Button("Close##FrustumCheckWinCloseBtn"))
        {
            m_bFrustumCheckWindow = false;
            ImGui::CloseCurrentPopup();
        };

        GetTarget()->Camera()->SetLayerFrustum(iFrustumLayer);

        ImGui::End();
    }
}

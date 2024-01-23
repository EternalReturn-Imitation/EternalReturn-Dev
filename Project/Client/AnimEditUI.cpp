#include "pch.h"
#include "AnimEditUI.h"

#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CResMgr.h>
#include <Engine/CDevice.h>

#include "CAnimEditObj.h"

#include <Engine/CRenderMgr.h>
#include <Engine/CMRT.h>

#include <Engine/CTexture.h>
#include <Engine/CStructuredBuffer.h>
#include <Script/CCameraMoveScript.h>

AnimEditUI::AnimEditUI()
    : UI("##AnimEditUI")
{
    SetName("AnimEditUI");


    // 에디터 윈도우 세팅
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        SetFlags(window_flags);
        SetModal(false);
    }
}

AnimEditUI::~AnimEditUI()
{
}

void AnimEditUI::tick()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    SetPopupPos(viewport->WorkPos);
    SetSize(viewport->WorkSize.x, viewport->WorkSize.y);
}

void AnimEditUI::finaltick()
{
    // 반투명 배경
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PopStyleVar();

    UI::finaltick();
}

int AnimEditUI::render_update()
{
    Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"3DAnimEditTex");

    int width = (int)pTex->Width();
    int height = (int)pTex->Height();

    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
    ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white

    ImGui::Image((ImTextureID)pTex->GetSRV().Get(), ImVec2(400, 400), uv_min, uv_max, tint_col, border_col);

    return 0;
}
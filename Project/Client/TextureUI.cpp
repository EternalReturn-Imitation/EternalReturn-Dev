#include "pch.h"
#include "TextureUI.h"

#include <Engine/CTexture.h>


TextureUI::TextureUI()
    : ResUI(RES_TYPE::TEXTURE)
{
    SetName("Texture");
}

TextureUI::~TextureUI()
{
}

int TextureUI::render_update()
{
    ResUI::render_update();


    if (RES_TYPE::TEXTURE == GetTargetRes()->GetType())
    {
        Ptr<CTexture> pTex = (CTexture*)GetTargetRes().Get();

        int width = pTex->Width();
        int height = pTex->Height();

        ImGui::Text("Width");
        ImGui::SameLine();
        ImGui::InputInt("##Width", &width, 50, ImGuiInputTextFlags_ReadOnly);

        ImGui::Text("Height");
        ImGui::SameLine();
        ImGui::InputInt("##Height", &height, 50, ImGuiInputTextFlags_ReadOnly);

        ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
        ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white

        ImGui::Image((ImTextureID)pTex->GetSRV().Get(), ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);
    }

    return 0;
}

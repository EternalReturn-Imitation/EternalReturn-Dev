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
    }

    return 0;
}

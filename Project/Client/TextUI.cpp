#include "pch.h"
#include "TextUI.h"

#include <Engine\CText.h>

TextUI::TextUI()
    : ComponentUI("##Text", COMPONENT_TYPE::TEXTCOMP)
{
    SetName("Text");
}

TextUI::~TextUI()
{
}

int TextUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

    CText* pText = GetTarget()->Text();


    ImVec2 TitleSize(70.f, 22.f);

    // Font
    // const char* font[4] = { "KBIZM", "Muli-SemiBold", "Roboto-Regular", "SairaSemiCondensed-Regular" };
    // int CurrentFont = (UINT)pText->GetFont();

    // Offset
    float vOffset[2] = { pText->GetOffsetPos().x, pText->GetOffsetPos().y };

    // FontSize
    float fFontSize = pText->GetFontSize();

    // FontColor
    ImVec4 vecColor = ImGui::ColorConvertU32ToFloat4(pText->GetFontColor());
    float FontColor[4] = { vecColor.x,vecColor.y,vecColor.z,vecColor.w };

    // wchar_t -> UTF-8
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    char szbuffer[256] = {};
    strcpy_s(szbuffer, converter.to_bytes(pText->GetString()).c_str());

    ImGui::PushItemWidth(-FLT_MIN);
    ImGui::InputText("##TextString", szbuffer, sizeof(szbuffer));

    // UTF-8 ->wchar_t
    std::string utf8String = szbuffer;
    std::wstring WString = converter.from_bytes(utf8String);

    // ImGui::Button("Font", TitleSize);
    // ImGui::SameLine();
    // ImGui::Combo("##Font", &CurrentFont, font, IM_ARRAYSIZE(font));

    ImGui::Button("Offset", TitleSize);
    ImGui::SameLine();
    ImGui::DragFloat2("##Offset", vOffset);

    ImGui::Button("FontSize", TitleSize);
    ImGui::SameLine();
    ImGui::SliderFloat("##FontSize", &fFontSize, 10.f, 300.f, "%.1f");

    ImGui::Button("FontColor", TitleSize);
    ImGui::SameLine();
    ImGui::ColorPicker4("##FontColor", FontColor);

    vecColor = { FontColor[0],FontColor[1],FontColor[2],FontColor[3] };
    UINT iFontColor = ImGui::ColorConvertFloat4ToU32(vecColor);

    // pText->SetFont((FONT)CurrentFont);
    pText->SetOffsetPos(Vec2(vOffset[0], vOffset[1]));
    pText->SetFontSize(fFontSize);
    pText->SetFontColor(iFontColor);
    pText->InputString(WString);

    return TRUE;
}

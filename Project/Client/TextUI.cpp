#include "pch.h"
#include "TextUI.h"

#include <Engine\CText.h>
#include <Engine\CFontMgr.h>

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

    CText* Textcomp = GetTarget()->Text();
    vector<CText::tTextInfo*> pText = Textcomp->m_vecTextInfo;

    // 프리팹 키
    ImGui::Button("Key", ImVec2(40.f, 0.f)); ImGui::SameLine();
    ImGui::SameLine();
    // wchar_t -> UTF-8
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    char szbuffer[256] = {};
    strcpy_s(szbuffer, converter.to_bytes(Textcomp->m_PrefabKey).c_str());

    ImGui::InputText("##PrefabKey", szbuffer, sizeof(szbuffer));

    // UTF-8 ->wchar_t
    std::string utf8String = szbuffer;
    std::wstring WString = converter.from_bytes(utf8String);

    Textcomp->m_PrefabKey = WString;    // 프리팹키 수정

    if (ImGui::Button("SAVE##TextPrepab"))
    {
        Textcomp->SavePrefab(Textcomp->m_PrefabKey);
    }

    ImGui::Separator();
    // 현재텍스트(Combo) / 전체 텍스트 수
    ImGui::Button("TextList", ImVec2(0.f, 0.f)); ImGui::SameLine();

    static int CurEditTex = 0;
    int MaxSize = pText.size();

    const char* idx[20] = { "0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19" };

    ImGui::SetNextItemWidth(60.f);
    if (ImGui::BeginCombo("##TextIdx", idx[CurEditTex], 0))
    {
        for (int n = 0; n < MaxSize; n++)
        {
            const bool is_selected = (CurEditTex == n);
            if (ImGui::Selectable(idx[n], is_selected))
            {
                CurEditTex = n;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();

    ImGui::Text(" / %d", pText.size());
    
    ImGui::SameLine();
    
    ImGui::BeginDisabled(20 <= MaxSize);
    if (ImGui::Button("+"))
    {
        Textcomp->AddText();
    }
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::BeginDisabled(MaxSize <= 1);
    if (ImGui::Button("-"))
    {
        if (CurEditTex == MaxSize - 1)
        {
            CurEditTex--;
        }
        Textcomp->DeleteLastIdx();
    }
    ImGui::EndDisabled();

    ImGui::Separator();
    
    if (MaxSize == 0)
        return true;

    ImVec2 TitleSize(70.f, 22.f);
    
    // Offset
    float vOffset[2] = { pText[CurEditTex]->vOffsetPos.x, pText[CurEditTex]->vOffsetPos.y};

    // FontSize
    float fFontSize = pText[CurEditTex]->fSize;

    // FontColor
    ImVec4 vecColor = ImGui::ColorConvertU32ToFloat4(pText[CurEditTex]->FontColor);
    float FontColor[4] = { vecColor.x,vecColor.y,vecColor.z,vecColor.w };

    // wchar_t -> UTF-8
    char texbuffer[256] = {};
    strcpy_s(texbuffer, converter.to_bytes(pText[CurEditTex]->str).c_str());

    ImGui::PushItemWidth(-FLT_MIN);
    ImGui::InputText("##TextString", texbuffer, sizeof(texbuffer));

    // UTF-8 ->wchar_t
    utf8String = texbuffer;
    WString = converter.from_bytes(utf8String);

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
    pText[CurEditTex]->vOffsetPos = Vec2(vOffset[0], vOffset[1]);
    pText[CurEditTex]->fSize = fFontSize;
    pText[CurEditTex]->FontColor = iFontColor;
    pText[CurEditTex]->str = WString;

    return TRUE;
}

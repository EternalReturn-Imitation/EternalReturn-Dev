#include "pch.h"
#include "CharacterDataUI.h"

#include "ER_CharacterMgr.h"
#include <Script\ER_DataScript_Character.h>

CharacterDataUI::CharacterDataUI()
    : UI("##CharacterDataUI")
    , m_pSelectedCharacter(nullptr)
{
    SetName("CharacterDataUI");

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    SetFlags(window_flags);
    SetModal(false);
}

CharacterDataUI::~CharacterDataUI()
{
}

void CharacterDataUI::RegistCharacters()
{
    m_pCharacters = &ER_CharacterMgr::GetInst()->m_mapCharacters;
    m_pSelectedCharacter = (*m_pCharacters).begin()->second;
}

void CharacterDataUI::init()
{
}

void CharacterDataUI::tick()
{
}

void CharacterDataUI::finaltick()
{
    // 에디터 윈도우 세팅
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    }

    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    SetPopupPos(viewport->WorkPos);
    SetSize(viewport->WorkSize.x, viewport->WorkSize.y);

    // 반투명 배경
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PopStyleVar(3);

    UI::finaltick();
}

int CharacterDataUI::render_update()
{
    render_menubar();
    
    if (!m_pCharacters)
        return 0;
    
    render_CharacterList();
    
    render_CharacterInfoData();
    

    return 0;
}

void CharacterDataUI::render_menubar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save.."))
                ER_CharacterMgr::GetInst()->Save();

            if (ImGui::MenuItem("Exit"))
                SetActive(false);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            ImGui::BeginDisabled(0 != m_pCharacters->size());
            
            if (ImGui::MenuItem("Add 5 Characters.."))
            {
                const wchar_t* CharNameList[5] = { L"Aya",L"Hyunwoo",L"Jackie",L"Rio",L"Yuki" };
                
                for (int i = 0; i < 5; ++i)
                {
                    CGameObject* NewCharacter = new CGameObject;
                    NewCharacter->AddComponent(new ER_DataScript_Character());
                    ER_DataScript_Character* CharacterContext = NewCharacter->GetScript<ER_DataScript_Character>();
                    
                    CharacterContext->m_strKey = CharNameList[i];
                    NewCharacter->SetName(CharacterContext->m_strKey);
                    CharacterContext->init();

                    m_pCharacters->insert(make_pair(CharNameList[i], NewCharacter));
                }
            }

            ImGui::EndDisabled();

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void CharacterDataUI::render_CharacterList()
{
    ImGui::BeginGroup();

    ImGui::Button("CharacterList", ImVec2(100.f, 0.f));
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;

    int CharacterCnt = 0;// (UINT)m_pCharacters->size();
    int CharacterIdx = 0;
    static int SelectedIdx = 0;
    
    ImGui::BeginChild("##CharacterList", ImVec2(100.f, 0.f), false, window_flags);

    ImGuiTreeNodeFlags NodeFlag = ImGuiTreeNodeFlags_Leaf;

    string strFinalName;

     map<wstring, CGameObject*>::iterator iter = m_pCharacters->begin();
     
     while (iter != m_pCharacters->end())
     {
         CharacterIdx++;
         
         const bool is_selected = (SelectedIdx == CharacterIdx);
     
         if (ImGui::Selectable(ToString(iter->first).c_str(), is_selected))
         {
             if (SelectedIdx != CharacterIdx)
             {
                 SelectedIdx = CharacterIdx;
     
                 m_pSelectedCharacter = iter->second;
             }
         }
     
         iter++;
     }


     ImGui::EndChild();

     ImGui::EndGroup();
}

void CharacterDataUI::render_CharacterInfoData()
{
    ER_DataScript_Character* CharacterContext = m_pSelectedCharacter->GetScript<ER_DataScript_Character>();
    float xsize = 61.f;
    
    // [ Full Tex ]
    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::Button("Character Tex", ImVec2(300.f, 0));
    
    ImVec2 CsrPos = ImGui::GetCursorPos();
    
    Ptr<CTexture> SelBgTex = CResMgr::GetInst()->FindRes<CTexture>(L"Char_Full_Circle.png");
    
    ImVec2 BgSize = { SelBgTex.Get()->Width(),SelBgTex.Get()->Width() };
    ImVec2 TexSize = { CharacterContext->m_FullTax.Get()->Width(),CharacterContext->m_FullTax.Get()->Height() };
    
    BgSize.y *= (300.f / BgSize.x); BgSize.x *= (300.f / BgSize.x);
    TexSize.y *= (300.f / TexSize.x); TexSize.x *= (300.f / TexSize.x);

    ImVec2 size = ImVec2(300.f, 300.f);
    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
    ImVec4 border_col = ImVec4(0.0f, 0.0f, 0.0f, 0.5f); // 50% opaque white

    ImTextureID SelectBGID = (CTexture*)SelBgTex->GetSRV().Get();
    ImGui::Image(SelectBGID, BgSize, uv_min, uv_max, tint_col, border_col);

    ImGui::SetCursorPos(CsrPos);
    
    ImTextureID TextureID = (CTexture*)CharacterContext->m_FullTax->GetSRV().Get();
    ImGui::Image(TextureID, TexSize, uv_min, uv_max, tint_col, border_col);


    ImGui::EndGroup();

    // [ name ]
    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::Button("Character Info", ImVec2(250, 0));

    // wchar_t -> UTF-8
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    char szbuffer[32] = {};
    strcpy_s(szbuffer, converter.to_bytes(CharacterContext->m_strName).c_str());

    ImGui::Button("Name",ImVec2(xsize,0.f));
    ImGui::SameLine();
    ImGui::SetNextItemWidth(181.f);
    ImGui::InputText("##CharacterName", szbuffer, sizeof(szbuffer));
    
    // UTF-8 ->wchar_t
    std::string utf8String = szbuffer;
    CharacterContext->m_strName = converter.from_bytes(utf8String);

    // [TexList]
    ImGui::Button("FullTex", ImVec2(xsize, 0.f)); ImGui::SetNextItemWidth(181.f); ImGui::SameLine();
    ImGui::Text(ToString(CharacterContext->m_FullTax.Get()->GetKey()).c_str());
    ImGui::Button("MapTex", ImVec2(xsize, 0.f)); ImGui::SetNextItemWidth(181.f); ImGui::SameLine();
    ImGui::Text(ToString(CharacterContext->m_MapTex.Get()->GetKey()).c_str());
    ImGui::Button("PortTex", ImVec2(xsize, 0.f)); ImGui::SetNextItemWidth(181.f); ImGui::SameLine();
    ImGui::Text(ToString(CharacterContext->m_PortraitTex.Get()->GetKey()).c_str());

    ImGui::EndGroup();


    // [ stats ]
    ImGui::SameLine();
    ImGui::BeginGroup();
    ER_Initial_Stats& stats = CharacterContext->m_STDStats;

    ImGui::Button("Character Stats", ImVec2(200.f, 0.f));

    ImGui::Button(" ", ImVec2(xsize, 0.f)); ImGui::SameLine();
    ImGui::Button("Initial", ImVec2(xsize, 0.f));; ImGui::SameLine();
    ImGui::Button("PerLV", ImVec2(xsize, 0.f));


    ImGui::Button("ATK", ImVec2(xsize, 0.f)); ImGui::SameLine();
    
    ImGui::SetNextItemWidth(xsize); ImGui::InputScalar("##CharAttackPower", ImGuiDataType_S16, &stats.iAttackPower); ImGui::SameLine();
    ImGui::SetNextItemWidth(xsize); ImGui::InputScalar("##CharAttackPowerPerLevel", ImGuiDataType_S16, &stats.iAttackPowerPerLevel);
    ImGui::Button("DEF", ImVec2(xsize, 0.f)); ImGui::SameLine();
    ImGui::SetNextItemWidth(xsize); ImGui::InputScalar("##CharDefense", ImGuiDataType_S16, &stats.iDefense); ImGui::SameLine();
    ImGui::SetNextItemWidth(xsize); ImGui::InputScalar("##CharDefensePerLevel", ImGuiDataType_S16, &stats.iDefensePerLevel);
    ImGui::Button("HP", ImVec2(xsize, 0.f)); ImGui::SameLine();
    ImGui::SetNextItemWidth(xsize); ImGui::InputScalar("##CharMaxHP", ImGuiDataType_S16, &stats.iMaxHP); ImGui::SameLine();
    ImGui::SetNextItemWidth(xsize); ImGui::InputScalar("##CharMaxHPPerLevel", ImGuiDataType_S16, &stats.iMaxHPPerLevel);
    ImGui::Button("HPgen", ImVec2(xsize, 0.f)); ImGui::SameLine();
    ImGui::SetNextItemWidth(xsize); ImGui::InputScalar("##CharHPRegen", ImGuiDataType_Float, &stats.fHPRegen); ImGui::SameLine();
    ImGui::SetNextItemWidth(xsize); ImGui::InputScalar("##CharHPRegenPerLevel", ImGuiDataType_Float, &stats.fHPRegenPerLevel);
    ImGui::Button("SP", ImVec2(xsize, 0.f)); ImGui::SameLine();
    ImGui::SetNextItemWidth(xsize); ImGui::InputScalar("##CharMaxSP", ImGuiDataType_S16, &stats.iMaxSP); ImGui::SameLine();
    ImGui::SetNextItemWidth(xsize); ImGui::InputScalar("##CharMaxSPPerLevel", ImGuiDataType_S16, &stats.iMaxSPPerLevel);
    ImGui::Button("SPgen", ImVec2(xsize, 0.f)); ImGui::SameLine();
    ImGui::SetNextItemWidth(xsize); ImGui::InputScalar("##CharSPRegen", ImGuiDataType_Float, &stats.fSPRegen); ImGui::SameLine();
    ImGui::SetNextItemWidth(xsize); ImGui::InputScalar("##CharSPRegenPerLevel", ImGuiDataType_Float, &stats.fSPRegenPerLevel);
    ImGui::Button("ATKSpd", ImVec2(xsize, 0.f)); ImGui::SameLine();
    ImGui::SetNextItemWidth(xsize); ImGui::InputScalar("##CharAttackSpeed", ImGuiDataType_Float, &stats.fAttackSpeed);
    ImGui::Button("CRI", ImVec2(xsize, 0.f)); ImGui::SameLine();
    ImGui::SetNextItemWidth(xsize); ImGui::InputScalar("##CharCriticalStrikeChance", ImGuiDataType_Float, &stats.fCriticalStrikeChance);
    ImGui::Button("SPD", ImVec2(xsize, 0.f)); ImGui::SameLine();
    ImGui::SetNextItemWidth(xsize); ImGui::InputScalar("##CharMovementSpeed", ImGuiDataType_Float, &stats.fMovementSpeed);
    ImGui::Button("VRange", ImVec2(xsize, 0.f)); ImGui::SameLine();
    ImGui::SetNextItemWidth(xsize); ImGui::InputScalar("##CharVisionRange", ImGuiDataType_Float, &stats.fVisionRange);

    ImGui::EndGroup();
}

void CharacterDataUI::render_ActionFuncLink()
{
}
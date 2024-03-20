#include "pch.h"
#include "CharacterDataUI.h"

#include <Script\ER_CharacterMgr.h>
#include <Script\ER_DataScript_Character.h>

CharacterDataUI::CharacterDataUI()
    : UI("##CharacterDataUI")
    , m_pSelectedCharacter(nullptr)
    , m_pSelectedSkill(nullptr)
    , m_iSelectedCharIdx(0)
    , m_iSelectedSkillIdx(0)
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

    render_SkillList();

    render_SkillInfoData();

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
    
    ImGui::BeginChild("##CharacterList", ImVec2(100.f, 300.f), false, window_flags);

    ImGuiTreeNodeFlags NodeFlag = ImGuiTreeNodeFlags_Leaf;

    string strFinalName;

     map<wstring, CGameObject*>::iterator iter = m_pCharacters->begin();
     
     while (iter != m_pCharacters->end())
     {
         CharacterIdx++;
         
         const bool is_selected = (m_iSelectedCharIdx == CharacterIdx);
     
         if (ImGui::Selectable(ToString(iter->first).c_str(), is_selected))
         {
             if (m_iSelectedCharIdx != CharacterIdx)
             {
                 m_iSelectedCharIdx = CharacterIdx;
     
                 m_pSelectedCharacter = iter->second;
                 m_iSelectedSkillIdx = 0;
                 m_pSelectedSkill = nullptr;
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


void CharacterDataUI::render_SkillList()
{
    ER_DataScript_Character* CharacterContext = m_pSelectedCharacter->GetScript<ER_DataScript_Character>();

    ImGui::BeginGroup();

    ImGui::Button("SkillList", ImVec2(200.f, 0.f));
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;

    int SkillCnt = 0;// (UINT)m_pCharacters->size();
    int SkillIdx = 0;

    ImGui::BeginChild("##SkillList", ImVec2(200.f, 176.f), false, window_flags);

    ImGuiTreeNodeFlags NodeFlag = ImGuiTreeNodeFlags_Leaf;

    string strFinalName;
    
    vector<ER_SKILL*>* vSkillList = &CharacterContext->m_SkillList;
    int SkillListSize = (int)(*vSkillList).size();

    for (int i = 0; i < SkillListSize; ++i)
    {
        SkillIdx++;

        const bool is_selected = (m_iSelectedSkillIdx == SkillIdx);

        string strName = ToString((*vSkillList)[i]->strName);
        const char* skillKey[8]{ "[Q_1]","[W_1]","[E_1]","[R_1]","[Q_2]","[W_2]","[E_2]","[R_2]" };
        strName += skillKey[i];

        if (ImGui::Selectable(strName.c_str(), is_selected))
        {
            if (m_iSelectedSkillIdx != SkillIdx)
            {
                m_iSelectedSkillIdx = SkillIdx;

                m_pSelectedSkill = (*vSkillList)[i];
            }
        }
    }

    ImGui::EndChild();

    ImGui::EndGroup();

}

void CharacterDataUI::render_SkillInfoData()
{
    ER_SKILL* SkillContext = m_pSelectedSkill;

    if (!SkillContext)
        return;

    float xsize = 61.f;

    // [ Full Tex ]
    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::Button("Character Tex", ImVec2(100.f, 0));

    ImVec2 CsrPos = ImGui::GetCursorPos();

    Ptr<CTexture> SelBgTex = CResMgr::GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_Empty.png");

    ImVec2 BgSize = { SelBgTex.Get()->Width(),SelBgTex.Get()->Width() };
    ImVec2 TexSize = { 100.f,100.f };

    ImVec2 size = ImVec2(100.f, 100.f);
    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
    ImVec4 border_col = ImVec4(0.0f, 0.0f, 0.0f, 0.5f); // 50% opaque white

    ImTextureID SelectBGID = (CTexture*)SelBgTex->GetSRV().Get();
    ImGui::Image(SelectBGID, BgSize, uv_min, uv_max, tint_col, border_col);


    if (SkillContext->TexSkill.Get())
    {
        ImGui::SetCursorPos(CsrPos);
        ImTextureID TextureID = (CTexture*)SkillContext->TexSkill->GetSRV().Get();
        ImGui::Image(TextureID, TexSize, uv_min, uv_max, tint_col, border_col);
    }


    

    ImGui::EndGroup();

    // [ name ]
    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::Button("Skill Info", ImVec2(250, 0));

    ImGui::Button("Name", ImVec2(xsize, 0.f));
    ImGui::SameLine();
    ImGui::SetNextItemWidth(181.f);
    ImGui::Text(ToString(SkillContext->strName).c_str());

    // [TexKey]
    ImGui::Button("SkillIconTex", ImVec2(xsize, 0.f)); ImGui::SetNextItemWidth(181.f); ImGui::SameLine();
    ImGui::Text(ToString(SkillContext->TexSkill.Get()->GetKey()).c_str());

    ImGui::EndGroup();

    // [ SkillInfo ]
    ImGui::SameLine();
    ImGui::BeginGroup();

    ImGui::Button("SkillInfo", ImVec2(200.f, 0.f));
    
    // MaxSkill Level
    ImGui::Button("MaxLv", ImVec2(xsize, 0.f)); ImGui::SameLine();
    ImGui::SetNextItemWidth(xsize * 2 - 5.f);
    if (ImGui::InputInt("##MaxSkillLevel",&SkillContext->iMaxSkillLevel))
    {
        SkillContext->iMaxSkillLevel = 5 < SkillContext->iMaxSkillLevel ? 5 : SkillContext->iMaxSkillLevel;
        SkillContext->iMaxSkillLevel = 1 > SkillContext->iMaxSkillLevel ? 1 : SkillContext->iMaxSkillLevel;
    }

    // Level Text
    ImGui::Button(" ", ImVec2(xsize, 0.f)); ImGui::SameLine();
    int SkillMaxLevel = SkillContext->iMaxSkillLevel;
    for (int i = 0; i < SkillMaxLevel; ++i)
    {
        char level[10] = {};
        sprintf_s(level, "Lv%d", i + 1);
        ImGui::Button(level, ImVec2(xsize, 0.f));

        if (i + 1 != SkillMaxLevel)
            ImGui::SameLine();
    }

    // [iValue1]
    ImGui::Button("INT_1", ImVec2(xsize, 0.f)); ImGui::SameLine();
    for (int i = 0; i < SkillMaxLevel; ++i)
    {
        char id[32] = {};
        sprintf_s(id, "##iValue1_%d", i);

        ImGui::SetNextItemWidth(xsize); ImGui::InputScalar(id, ImGuiDataType_S32, &SkillContext->iValue1[i]);

        if (i + 1 != SkillMaxLevel)
            ImGui::SameLine();
    }

    // [iValue1]
    ImGui::Button("INT_2", ImVec2(xsize, 0.f)); ImGui::SameLine();
    for (int i = 0; i < SkillMaxLevel; ++i)
    {
        char id[32] = {};
        sprintf_s(id, "##iValue2_%d", i);

        ImGui::SetNextItemWidth(xsize); ImGui::InputScalar(id, ImGuiDataType_S32, &SkillContext->iValue2[i]);

        if (i + 1 != SkillMaxLevel)
            ImGui::SameLine();
    }

    // [fValue1]
    ImGui::Button("FLT_1", ImVec2(xsize, 0.f)); ImGui::SameLine();
    for (int i = 0; i < SkillMaxLevel; ++i)
    {
        char id[32] = {};
        sprintf_s(id, "##fValue1_%d", i);

        ImGui::SetNextItemWidth(xsize); ImGui::InputFloat(id, &SkillContext->fValue1[i]);

        if (i + 1 != SkillMaxLevel)
            ImGui::SameLine();
    }

    // [fValue2]
    ImGui::Button("FLT_2", ImVec2(xsize, 0.f)); ImGui::SameLine();
    for (int i = 0; i < SkillMaxLevel; ++i)
    {
        char id[32] = {};
        sprintf_s(id, "##fValue2_%d", i);

        ImGui::SetNextItemWidth(xsize); ImGui::InputFloat(id, &SkillContext->fValue2[i]);

        if (i + 1 != SkillMaxLevel)
            ImGui::SameLine();
    }

    // [fRange]
    ImGui::Button("RNG", ImVec2(xsize, 0.f)); ImGui::SameLine();
    for (int i = 0; i < SkillMaxLevel; ++i)
    {
        char id[32] = {};
        sprintf_s(id, "##fRange_%d", i);

        ImGui::SetNextItemWidth(xsize); ImGui::InputFloat(id, &SkillContext->fRange[i]);

        if (i + 1 != SkillMaxLevel)
            ImGui::SameLine();
    }

    
    // [fCoolDown]
    ImGui::Button("COOL", ImVec2(xsize, 0.f)); ImGui::SameLine();
    for (int i = 0; i < SkillMaxLevel; ++i)
    {
        char id[32] = {};
        sprintf_s(id, "##fCooldown_%d", i);

        ImGui::SetNextItemWidth(xsize); ImGui::InputFloat(id, &SkillContext->fMaxCoolDown[i]);

        if (i + 1 != SkillMaxLevel)
            ImGui::SameLine();
    }

    ImGui::EndGroup();
}
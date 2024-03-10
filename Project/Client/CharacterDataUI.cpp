#include "pch.h"
#include "CharacterDataUI.h"

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
    
    // if (!m_pCharacters)
    //     return 0;
    
    render_CharacterList();

    return 0;
}

void CharacterDataUI::render_menubar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
                SetActive(false);

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void CharacterDataUI::render_CharacterList()
{
    ImGui::Button("CharacterList", ImVec2(300.f, 0.f));
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;

    int CharacterCnt = (UINT)m_pCharacters->size();
    int CharacterIdx = 0;
    static int SelectedIdx = 0;

    ImGui::BeginChild("##CharacterList", ImVec2(200.f, 600.f), false, window_flags);

    ImGuiTreeNodeFlags NodeFlag = ImGuiTreeNodeFlags_Leaf;

    string strFinalName;

    map<wstring, ER_Character*>::iterator iter = m_pCharacters->begin();

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


}

void CharacterDataUI::render_CharacterInfoData()
{
}

void CharacterDataUI::render_ActionFuncLink()
{
}
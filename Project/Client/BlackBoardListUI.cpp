#include "pch.h"
#include "BlackBoardListUI.h"

#include <Engine\CBehaviorTree.h>
#include <Engine\CBehaviorTreeMgr.h>

BlackBoardListUI::BlackBoardListUI()
	: UI("##BlackBoardList")
	, m_pTargetBB(nullptr)
{
	SetName("BlackBoardList");
}

BlackBoardListUI::~BlackBoardListUI()
{
}

void BlackBoardListUI::init()
{
	SetBlackBoard(nullptr);
}

void BlackBoardListUI::tick()
{
}

int BlackBoardListUI::render_update()
{
    if (m_pTargetBB == nullptr)
        return 0;

    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
    const float VALUE_MAX_WIDTH = ImGui::CalcTextSize("{ 0.000, 0.000, 0.000, 0.000 }").x;
    

    // static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
    static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
    
    
    if (ImGui::BeginTable("BBTable", 3, flags))
    {
        ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_NoHide);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, VALUE_MAX_WIDTH);
        ImGui::TableHeadersRow();

        const char* DataKey = nullptr;
        const char* DataType = nullptr;
        string DataValue;

        list<tBBKey> BBList = m_pTargetBB->GetKeyList();
        list<tBBKey>::iterator iter = BBList.begin();

        for (auto BBinfo : BBList)
        {
            DataKey   = BBinfo.Key.c_str();
            DataType  = BBinfo.GetType();
            m_pTargetBB->GetvalueStr(BBinfo, DataValue);
            
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            ImGui::TreeNodeEx(DataKey, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
            ImGui::TableNextColumn();
            ImGui::Text(DataType);
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(DataValue.c_str());
        }
        ImGui::EndTable();
    }

	return 0;
}

void BlackBoardListUI::SetBlackBoard(BB* _Target)
{
	m_pTargetBB = _Target;
}


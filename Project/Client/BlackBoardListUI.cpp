#include "pch.h"
#include "BlackBoardListUI.h"

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

        unordered_map<string, BB::tBBData*> BBList = m_pTargetBB->GetBBList();

        for (auto iter : BBList)
        {
            
            DataKey     = iter.second->strKey.c_str();
            DataType    = iter.second->strDataType;
            DataValue   = GetDataStr(iter.second, DataType);
            
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

string BlackBoardListUI::GetDataStr(BB::tBBData* _Data, const char* _DataType)
{
    string res;

    if (_DataType == "int")
    {
        res = std::to_string(*(int*)_Data->pDataPtr);
    }
    else if (_DataType == "float")
    {
        char fstr[10] = {};
        float f = *((float*)_Data->pDataPtr);

        sprintf_s(fstr, "%.3f", f);
        res = fstr;
    }
    else if (_DataType == "GameObject")
    {
        wstring ObjName = ((CGameObject*)_Data->pDataPtr)->GetName();
        res.assign(ObjName.begin(), ObjName.end());
    }
    else if (_DataType == "string")
    {
        res = _Data->strData;
    }
    else if (_DataType == "wstring")
    {
        res = _Data->strData;
    }

    return res;
}


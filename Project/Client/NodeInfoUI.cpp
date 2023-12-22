#include "pch.h"
#include "NodeInfoUI.h"

#include <Engine\CBehaviorTree.h>
#include <Engine\CBehaviorTreeMgr.h>

#include "BehaviorTreeListUI.h"
#include "TreeUI.h"

NodeInfoUI::NodeInfoUI()
    : UI("##NodeInfo")
    , m_pTargetNode(nullptr)
{
    SetName("NodeInfo");
}

NodeInfoUI::~NodeInfoUI()
{
}

void NodeInfoUI::init()
{
    SetTargetNode(nullptr);
}

void NodeInfoUI::tick()
{
    
}

int NodeInfoUI::render_update()
{
    if (m_pTargetNode == nullptr)
        return FALSE;

    BTNode* pNode = m_pTargetNode;

    BTNode::NODETYPE eNodeType = pNode->GetNodeType();

    string strNodeType;
    UINT TypeColor = 0;
    bool bUseFlag = false;
    
    
    switch (eNodeType)
    {
    case BTNode::NODETYPE::ROOT:
        strNodeType = "ROOT";
        TypeColor = 0;
        break;
    case BTNode::NODETYPE::COMPOSITE:
        strNodeType = "COMPOSITE";
        TypeColor = 4;
        bUseFlag = true;
        break;
    case BTNode::NODETYPE::DECORATOR:
        strNodeType = "DECORATOR";
        TypeColor = 3;
        bUseFlag = true;
        break;
    case BTNode::NODETYPE::TASK:
        strNodeType = "TASK";
        TypeColor = 5;
        bUseFlag = true;
        break;
    }
    
    strNodeType = "NodeType : " + strNodeType + " NODE";

    wstring tmp = pNode->GetNodeName();
    string strNodeName;
    strNodeName.assign(tmp.begin(), tmp.end());

    ImGui::PushID(0);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(TypeColor / 7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(TypeColor / 7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(TypeColor / 7.0f, 0.6f, 0.6f));
    ImGui::Button(strNodeName.c_str());
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    ImGui::Text(strNodeType.c_str());
    
    if (bUseFlag)
    {
        ImGui::Text("Flags : ");
        ImGui::SameLine();
        
        int iCurrnetFlag = pNode->GetNodeFlag();
        const char** FlagsList = pNode->GetFlagList();
        ImGui::Combo("##NodeFlags", &iCurrnetFlag, FlagsList, pNode->GetFlagCnt());
        pNode->SetNodeFlag(iCurrnetFlag);
    }

    return TRUE;
}

void NodeInfoUI::SetTargetNode(BTNode* _Target)
{
    m_pTargetNode = _Target;
}

void NodeInfoUI::ClearTargetNode()
{
    m_pTargetNode = nullptr;
}
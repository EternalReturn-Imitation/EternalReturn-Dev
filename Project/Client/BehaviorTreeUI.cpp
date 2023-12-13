#include "pch.h"
#include "BehaviorTreeUI.h"

#include <Engine\CBehaviorTree.h>

#include "ImGuiMgr.h"

#include "NodeLinkerUI.h"
#include "TreeUI.h" 

BehaviorTreeUI::BehaviorTreeUI()
    : ComponentUI("##BehaviorTree", COMPONENT_TYPE::BEHAVIORTREE)
{
    SetName("BehaviorTree");
}

BehaviorTreeUI::~BehaviorTreeUI()
{
}

int BehaviorTreeUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

    char szBuff[50] = {};
    
    BTNode* RootNode = GetTarget()->BehaviorTree()->GetRootNode();
    ImGui::Text("BehaivorTree Editor");

    if (ImGui::Button("Open", ImVec2(100, 18)))
    {
        NodeLinkerUI* pTreeUI = (NodeLinkerUI*)ImGuiMgr::GetInst()->FindUI("##NodeLinker");
        pTreeUI->SetName("BTNodeTree");
        pTreeUI->SetActive(true);
        pTreeUI->SetRootNode(RootNode);

        pTreeUI->ResetNodeLinker();
    }

    return TRUE;
}
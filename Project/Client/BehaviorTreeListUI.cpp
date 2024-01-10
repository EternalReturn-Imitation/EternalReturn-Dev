#include "pch.h"
#include "BehaviorTreeListUI.h"

#include "ImGuiMgr.h"
#include "InspectorUI.h"
#include "NodeInfoUI.h"
#include "BlackBoardListUI.h"

#include <Engine/CBehaviorTree.h>

#include "TreeUI.h"

BehaviorTreeListUI::BehaviorTreeListUI()
	: UI("##BehaviorTreeList")
	, m_Tree(nullptr)
	, m_dwSelectedData(0)
	, m_RootNode(0)
	, m_NodeInfo(nullptr)
	, m_BBList(nullptr)
{
	SetName("BehaviorTreeList");
	
	m_Tree = new TreeUI;
	m_Tree->SetName("BehaviorTree");
	
	m_Tree->ShowRoot(true);
	m_Tree->ShowArrowBtn(true);
	m_Tree->ShowGroupIdx(true);

	m_Tree->AddDynamic_Select(this, (UI_DELEGATE_1)&BehaviorTreeListUI::SetTargetToInspector);
	m_Tree->AddDynamic_DragDrop(this, (UI_DELEGATE_2)&BehaviorTreeListUI::DragDrop);
	m_Tree->AddDynamic_SwapFront(this, (UI_DELEGATE_1)&BehaviorTreeListUI::SwapNodeFront);
	m_Tree->AddDynamic_SwapBack(this, (UI_DELEGATE_1)&BehaviorTreeListUI::SwapNodeBack);

	m_Tree->SetDragDropID("BTNode");

	m_NodeInfo = new NodeInfoUI;
	m_BBList = new BlackBoardListUI;
	m_BBList->SetFlags(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar);
	m_BBList->SetActive(true);
	// m_BBList->SetFlags(ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_AlwaysVerticalScrollbar);
	
	// m_NodeInfo->SetActive(true);

	// AddChildUI(m_Tree);
	// AddChildUI(m_NodeInfo);
}

BehaviorTreeListUI::~BehaviorTreeListUI()
{
	delete m_Tree;
	delete m_NodeInfo;
	delete m_BBList;
}

void BehaviorTreeListUI::tick()
{
	// m_NodeInfo->SetActive(true);
	m_Tree->tick();
	m_NodeInfo->tick();
	m_BBList->tick();
}

void BehaviorTreeListUI::finaltick()
{
	UI::finaltick();
	m_BBList->finaltick();
}

int BehaviorTreeListUI::render_update()
{
	ImVec2 size = ImGui::GetWindowSize();
	string strFullName = m_Tree->GetName() + m_Tree->GetID();
	{
		int NodeInfoWidth = 250;

		ImGui::GetWindowSize();
		ImGui::BeginChild(strFullName.c_str(), ImVec2((float)(size.x - (NodeInfoWidth + 40)) , -ImGui::GetFrameHeightWithSpacing()));
		ImGui::BeginGroup();
		m_Tree->render_update();
		ImGui::EndGroup();
		ImGui::EndChild();

		ImGui::SameLine();
		{
			ImGui::BeginGroup();
			strFullName = m_NodeInfo->GetName() + m_NodeInfo->GetID();
			ImGui::BeginChild(strFullName.c_str(), ImVec2((float)NodeInfoWidth, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
			m_NodeInfo->render_update();
			ImGui::EndChild();
			if (ImGui::Button("Save")) {}
			ImGui::SameLine();
			if (ImGui::Button("Load")) {}
			ImGui::EndGroup();
		}
	}
	ImGui::SetNextWindowSize(ImVec2(450.f, 300.f));

	return 0;
}

void BehaviorTreeListUI::OpenBlackBoardListUI()
{
	m_BBList->SetActive(true);
}

void BehaviorTreeListUI::ResetNodeLinker()
{
	SetSize(300.f, 500.f);

	// Tree Clear
	m_Tree->Clear();

	// 지정된 루트노드로부터 파생된 모든 노드 받아옴
	if(m_RootNode)
		AddNode((BTNode*)m_RootNode, nullptr);
}

void BehaviorTreeListUI::SetTargetObject(CGameObject* _Target)
{
	// ClearTargetResource()

	// 새 타겟
	CComponent* Com = _Target->BehaviorTree();
	if (Com)
	{
		BTNode* Root = _Target->BehaviorTree()->GetRootNode();
		m_RootNode = (DWORD_PTR)Root;
		m_BBList->SetBlackBoard(((Root_Node*)Root)->GetBlackBoard());
	}

	else
	{
		m_RootNode = 0;
		m_NodeInfo->SetTargetNode(nullptr);
		m_BBList->SetBlackBoard(nullptr);
	}
	
	ResetNodeLinker();
}

void BehaviorTreeListUI::SetTargetToInspector(DWORD_PTR _SelectedNode)
{
	TreeNode* pSelectedNode = (TreeNode*)_SelectedNode;

	BTNode* SelectNode = (BTNode*)pSelectedNode->GetData();
	m_NodeInfo->SetTargetNode(SelectNode);
}

BTNode* BehaviorTreeListUI::GetSelectedNode()
{
	return m_NodeInfo->GetTargetNode();
}

void BehaviorTreeListUI::AddNode(BTNode* _Node, TreeNode* _ParentNode)
{

	// 노드를 트리에 넣고, 생성된 노드 주소를 받아둔다.
	TreeNode* pNode = m_Tree->AddItem(string(_Node->GetNodeName().begin(), _Node->GetNodeName().end())
									, (DWORD_PTR)_Node
									, _ParentNode);


	switch (_Node->GetNodeType())
	{
	case BT_ROOT:
		pNode->SetNodeColor(0);
		break;
	case BT_COMPOSITE:
		pNode->SetNodeColor(4);
		break;
	case BT_TASK:
		pNode->SetNodeColor(5);
		break;
	}
	

	int	NodeGroupIdx = 1;
	
	if (_ParentNode != nullptr)
	{
		NodeGroupIdx = _ParentNode->GetChildNodeSize() + 1;
		_ParentNode->SetChildNodeSize(NodeGroupIdx);
	}

	pNode->SetGroupIdx(NodeGroupIdx);


	list<BTNode*> ChildNodes = _Node->GetChild();
	if (!(ChildNodes.empty()))
	{
		list<BTNode*>::iterator iter = ChildNodes.begin();

		for (; iter != ChildNodes.end(); ++iter)
		{
			AddNode(*iter, pNode);
			NodeGroupIdx++;
		}
	}
}

void BehaviorTreeListUI::DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode)
{
	TreeNode* pDragNode = (TreeNode*)_DragNode;
	TreeNode* pDropNode = (TreeNode*)_DropNode;

	BTNode* pDragBT = (BTNode*)pDragNode->GetData();
	BTNode* pDropBT = nullptr;
	if (nullptr != pDropNode)
	{
		pDropBT = (BTNode*)pDropNode->GetData();
	}

	// 자식으로 들어갈 오브젝트가 목적지 오브젝트의 조상(부모계층) 중 하나라면, 
	// AddChild 처리하지 않는다.
	if (nullptr != pDropBT)
	{
		if (pDropBT->IsAncestorNode(pDragBT))
			return;
	}
	else if (nullptr == pDropBT)
		return;

	// 드롭노드에 자식이 있는데 드래그노드가 task인경우
	if (0 < pDropBT->GetChildCnt() 
		&& pDropBT->GetNodeType() != BT_COMPOSITE
		&& pDragBT->GetNodeType() == BT_TASK)
		return;

	pDropBT->AddChild(pDragBT);

	// Tree Update
	m_Tree->Clear();
	AddNode((BTNode*)m_RootNode, nullptr);
}

void BehaviorTreeListUI::SwapNodeFront(DWORD_PTR _pNode)
{
	TreeNode* pNode = (TreeNode*)_pNode;
	BTNode* pBTNode = (BTNode*)pNode->GetData();

	pBTNode->SwapFront();

	// Tree Update
	m_Tree->Clear();
	AddNode((BTNode*)m_RootNode, nullptr);
}

void BehaviorTreeListUI::SwapNodeBack(DWORD_PTR _pNode)
{
	TreeNode* pNode = (TreeNode*)_pNode;
	BTNode* pBTNode = (BTNode*)pNode->GetData();

	pBTNode->SwapBack();

	// Tree Update
	m_Tree->Clear();
	AddNode((BTNode*)m_RootNode, nullptr);
}
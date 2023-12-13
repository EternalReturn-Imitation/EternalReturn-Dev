#include "pch.h"
#include "BehaviorTreeUI.h"

#include "ImGuiMgr.h"
#include "InspectorUI.h"

#include <Engine\CBehaviorTree.h>

#include "TreeUI.h"

BehaviorTreeUI::BehaviorTreeUI()
	: UI("##BehaviorTree")
	, m_Tree(nullptr)
	, m_dwSelectedData(0)
	, m_RootNode(0)
{
	SetName("BehaviorTree");
	
	m_Tree = new TreeUI;
	m_Tree->SetName("BehaviorTree");
	m_Tree->SetActive(true);
	m_Tree->ShowRoot(true);
	m_Tree->ShowArrowBtn(true);

	m_Tree->AddDynamic_Select(this, (UI_DELEGATE_1)&BehaviorTreeUI::SetTargetToInspector);
	m_Tree->AddDynamic_DragDrop(this, (UI_DELEGATE_2)&BehaviorTreeUI::DragDrop);
	m_Tree->SetDragDropID("BTNode");

	AddChildUI(m_Tree);
}

BehaviorTreeUI::~BehaviorTreeUI()
{
}

void BehaviorTreeUI::tick()
{
}

int BehaviorTreeUI::render_update()
{
	return 0;
}

void BehaviorTreeUI::ResetNodeLinker()
{
	SetSize(300.f, 500.f);

	// Tree Clear
	m_Tree->Clear();

	// 지정된 루트노드로부터 파생된 모든 노드 받아옴
	if(m_RootNode)
		AddNode((BTNode*)m_RootNode, nullptr);
}

void BehaviorTreeUI::SetTargetObject(CGameObject* _Target)
{
	// ClearTargetResource()

	// 새 타겟
	CComponent* Com = _Target->BehaviorTree();
	if (Com)
		m_RootNode = (DWORD_PTR)_Target->BehaviorTree()->GetRootNode();

	else
		m_RootNode = 0;
	
	ResetNodeLinker();
}

void BehaviorTreeUI::SetTargetToInspector(DWORD_PTR _SelectedNode)
{
	TreeNode* pSelectedNode = (TreeNode*)_SelectedNode;
}

BTNode* BehaviorTreeUI::GetSelectedNode()
{
	return nullptr;
}

void BehaviorTreeUI::AddNode(BTNode* _Node, TreeNode* _ParentNode)
{
	// 노드를 트리에 넣고, 생성된 노드 주소를 받아둔다.
	TreeNode* pNode = m_Tree->AddItem(string(_Node->GetNodeName().begin(), _Node->GetNodeName().end())
									, (DWORD_PTR)_Node
									, _ParentNode);

	// 노드의 자식노드들을 기존 노드를 부모로해서 그 밑으로 다시 넣어준다.
	BTNode* ChildNode = _Node->GetChildNode();
	if (ChildNode)
		AddNode(ChildNode, pNode);

	list<BTNode*> ChildNodes = _Node->GetChildNodes();
	if (!(ChildNodes.empty()))
	{
		list<BTNode*>::iterator iter = ChildNodes.begin();

		for (; iter != ChildNodes.end(); ++iter)
		{
			AddNode(*iter, pNode);
		}
	}
}

void BehaviorTreeUI::DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode)
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
		if (pDropBT->IsAncestor(pDragBT))
			return;
	}

	// 행동트리 노드 변경
	if ((pDragBT->GetParentNode() == pDropBT)
		|| (NODETYPE::COMPOSITE != pDropBT->GetNodeType())
		|| (NODETYPE::ROOT == pDropBT->GetNodeType()))
		return;

	Composite_Node* ParentNode = dynamic_cast<Composite_Node*>(pDropBT);
	pDragBT->DisconnectFromParent();

	ParentNode->AddChildNode(pDragBT);

	// AddChild
	m_Tree->Clear();
	AddNode((BTNode*)m_RootNode, nullptr);
}


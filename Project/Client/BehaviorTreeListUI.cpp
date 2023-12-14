#include "pch.h"
#include "BehaviorTreeListUI.h"

#include "ImGuiMgr.h"
#include "InspectorUI.h"

#include <Engine\CBehaviorTree.h>

#include "TreeUI.h"

BehaviorTreeListUI::BehaviorTreeListUI()
	: UI("##BehaviorTreeList")
	, m_Tree(nullptr)
	, m_dwSelectedData(0)
	, m_RootNode(0)
{
	SetName("BehaviorTreeList");
	
	m_Tree = new TreeUI;
	m_Tree->SetName("BehaviorTree");
	m_Tree->SetActive(true);
	m_Tree->ShowRoot(true);
	m_Tree->ShowArrowBtn(true);
	m_Tree->ShowGroupIdx(true);

	m_Tree->AddDynamic_Select(this, (UI_DELEGATE_1)&BehaviorTreeListUI::SetTargetToInspector);
	m_Tree->AddDynamic_DragDrop(this, (UI_DELEGATE_2)&BehaviorTreeListUI::DragDrop);
	m_Tree->SetDragDropID("BTNode");


	AddChildUI(m_Tree);
}

BehaviorTreeListUI::~BehaviorTreeListUI()
{
}

void BehaviorTreeListUI::tick()
{
}

int BehaviorTreeListUI::render_update()
{
	return 0;
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
		m_RootNode = (DWORD_PTR)_Target->BehaviorTree()->GetRootNode();

	else
		m_RootNode = 0;
	
	ResetNodeLinker();
}

void BehaviorTreeListUI::SetTargetToInspector(DWORD_PTR _SelectedNode)
{
	TreeNode* pSelectedNode = (TreeNode*)_SelectedNode;
}

BTNode* BehaviorTreeListUI::GetSelectedNode()
{
	return nullptr;
}

void BehaviorTreeListUI::AddNode(BTNode* _Node, TreeNode* _ParentNode)
{

	// 노드를 트리에 넣고, 생성된 노드 주소를 받아둔다.
	TreeNode* pNode = m_Tree->AddItem(string(_Node->GetNodeName().begin(), _Node->GetNodeName().end())
									, (DWORD_PTR)_Node
									, _ParentNode);


	switch (_Node->GetNodeType())
	{
	case NODETYPE::ROOT:
		pNode->SetNodeColor(0);
		break;
	case NODETYPE::COMPOSITE:
		pNode->SetNodeColor(4);
		break;
	case NODETYPE::TASK:
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


	// 노드의 자식노드들을 기존 노드를 부모로해서 그 밑으로 다시 넣어준다.
	BTNode* ChildNode = _Node->GetChildNode();
	if (ChildNode)
	{
		AddNode(ChildNode, pNode);
	}

	list<BTNode*> ChildNodes = _Node->GetChildNodes();
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
		if (pDropBT->IsAncestor(pDragBT))
			return;
	}
	else if (nullptr == pDropBT)
		return;

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
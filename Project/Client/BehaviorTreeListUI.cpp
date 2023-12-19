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

	// ������ ��Ʈ���κ��� �Ļ��� ��� ��� �޾ƿ�
	if(m_RootNode)
		AddNode((BTNode*)m_RootNode, nullptr);
}

void BehaviorTreeListUI::SetTargetObject(CGameObject* _Target)
{
	// ClearTargetResource()

	// �� Ÿ��
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

	// ��带 Ʈ���� �ְ�, ������ ��� �ּҸ� �޾Ƶд�.
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


	// ����� �ڽĳ����� ���� ��带 �θ���ؼ� �� ������ �ٽ� �־��ش�.
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

	// �ڽ����� �� ������Ʈ�� ������ ������Ʈ�� ����(�θ����) �� �ϳ����, 
	// AddChild ó������ �ʴ´�.
	if (nullptr != pDropBT)
	{
		if (pDropBT->IsAncestor(pDragBT))
			return;
	}
	else if (nullptr == pDropBT)
		return;

	// �ൿƮ�� ��� ����
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
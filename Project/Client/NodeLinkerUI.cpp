#include "pch.h"
#include "NodeLinkerUI.h"

#include "ImGuiMgr.h"
#include "InspectorUI.h"

#include <Engine\CBehaviorTree.h>

#include "TreeUI.h"

NodeLinkerUI::NodeLinkerUI()
	: UI("##NodeLinker")
	, m_Tree(nullptr)
	, m_dwSelectedData(0)
	, m_RootNode(0)
{
	SetName("NodeLinker");
	
	m_Tree = new TreeUI;
	m_Tree->SetName("NodeLinkerTree");
	m_Tree->SetActive(true);
	m_Tree->ShowRoot(true);

	m_Tree->AddDynamic_Select(this, (UI_DELEGATE_1)&NodeLinkerUI::SetTargetToInspector);
	m_Tree->AddDynamic_DragDrop(this, (UI_DELEGATE_2)&NodeLinkerUI::DragDrop);
	m_Tree->SetDragDropID("BTNode");

	AddChildUI(m_Tree);
}

NodeLinkerUI::~NodeLinkerUI()
{
}

void NodeLinkerUI::tick()
{
}

int NodeLinkerUI::render_update()
{
	return 0;
}

void NodeLinkerUI::ResetNodeLinker()
{
	SetSize(300.f, 500.f);

	// Tree Clear
	m_Tree->Clear();

	// 지정된 루트노드로부터 파생된 모든 노드 받아옴
	if(m_RootNode)
		AddNode((BTNode*)m_RootNode, nullptr);
}

void NodeLinkerUI::SetTargetObject(CGameObject* _Target)
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

void NodeLinkerUI::SetTargetToInspector(DWORD_PTR _SelectedNode)
{
	TreeNode* pSelectedNode = (TreeNode*)_SelectedNode;
}

BTNode* NodeLinkerUI::GetSelectedNode()
{
	return nullptr;
}

void NodeLinkerUI::AddNode(BTNode* _Node, TreeNode* _ParentNode)
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

void NodeLinkerUI::DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode)
{
}


#include "pch.h"
#include "CBehaviorTree.h"

BTNode::~BTNode()
{
	DELETE_UNVAILUBLE(m_ChildNode);
	
	for (const auto& node : m_ChildNodes)
	{
		delete node;
	}
}

CBehaviorTree::CBehaviorTree()
	: CComponent(COMPONENT_TYPE::BEHAVIORTREE)
	, m_RootNode(nullptr)
{
	m_RootNode = new Root_Node;
	m_RootNode->SetNodeName(L"NewBehaviorTree");
	m_RootNode->SetBehaviorTree(this);
}

CBehaviorTree::~CBehaviorTree()
{
	DELETE_UNVAILUBLE(m_RootNode);
}

BTNode* CBehaviorTree::CreateNewNode(NODETYPE eType)
{
	BTNode* NewNode = nullptr;

	switch (eType)
	{
	case NODETYPE::SELECTOR:
		NewNode = new Selector();
		break;
	case NODETYPE::SEQUENCE:
		NewNode = new Sequence();
		break;
	case NODETYPE::RANDSELECTOR:
		NewNode = new RandomSelector();
		break;
	case NODETYPE::TASK:
		NewNode = new Task_Node();
		break;
	//case NODETYPE::COMPARE_BBENTRIES:
	//	break;
	//case NODETYPE::CUSTOM_TASK:
	//	break;
	//case NODETYPE::PLAY_ANIMATION:
	//	break;
	//case NODETYPE::PLAY_SOUND:
	//	break;
	//case NODETYPE::WAIT:
	//	break;
	//case NODETYPE::WAIT_BLACKBOARD_TIME:
	//	break;
	default:
		assert(nullptr);	// 노드타입 설정 오류
		break;
	}

	return NewNode;
}

void CBehaviorTree::tick()
{
	// 게임 플레이시 노드 순회
	m_RootNode->Run();
}

void CBehaviorTree::finaltick()
{
}

BTNode* CBehaviorTree::AddChildNode(BTNode* _ParentNode, NODETYPE eType)
{

	BTNode* NewChildNode = CreateNewNode(eType);
	NODETYPE NewNodeType = NewChildNode->GetNodeType();

	if (NewNodeType == NODETYPE::DECORATOR || NewNodeType == NODETYPE::SERVICE)
		assert(nullptr);

	if (dynamic_cast<Root_Node*>(_ParentNode))
	{
		((Root_Node*)_ParentNode)->AddChildNode(NewChildNode);
	}
	else if (dynamic_cast<Composite_Node*>(_ParentNode))
	{
		((Composite_Node*)_ParentNode)->AddChildNode(NewChildNode);
	}
	else
		assert(nullptr);
	
	NewChildNode->SetParentNode(_ParentNode);

	return NewChildNode;
}

BTNode* CBehaviorTree::NodeAttach(BTNode* TargetNode, NODETYPE eType)
{
	BTNode* NewAttachNode = CreateNewNode(eType);
	NODETYPE NewNodeType = NewAttachNode->GetNodeType();

	if (NewNodeType == NODETYPE::COMPOSITE || NewNodeType == NODETYPE::TASK)
		assert(nullptr);

	if (dynamic_cast<Composite_Node*>(TargetNode))
	{
		((Composite_Node*)TargetNode)->NodeAttach(NewAttachNode);
	}
	else if (dynamic_cast<Task_Node*>(TargetNode))
	{
		((Task_Node*)TargetNode)->NodeAttach(NewAttachNode);
	}
	else
		assert(nullptr);

	return NewAttachNode;
}

void BTNode::DisconnectFromParent()
{
	if (!m_ParentNode)
		return;

	if (m_ParentNode->m_ChildNode)
	{
		m_ParentNode->m_ChildNode = nullptr;
		m_ParentNode = nullptr;
		return;
	}

	list<BTNode*>::iterator iter = m_ParentNode->m_ChildNodes.begin();
	for (; iter != m_ParentNode->m_ChildNodes.end(); ++iter)
	{
		if (this == *iter)
		{
			m_ParentNode->m_ChildNodes.erase(iter);
			m_ParentNode = nullptr;
			return;
		}
	}

	assert(nullptr);
}



BT_STATUS Selector::Run()
{
	if (Composite_Node::Run() == BT_STATUS::FAILURE)
		return BT_STATUS::FAILURE;

	for (BTNode* child : m_ChildNodes)
	{
		BT_STATUS res = child->Run();

		if (res == BT_STATUS::SUCCESS)
			return BT_STATUS::SUCCESS;

		if (res == BT_STATUS::RUNNING)
		{
			((Root_Node*)GetBehaviorTree()->GetRootNode())->SetRunningNode(child);
			return BT_STATUS::SUCCESS;
		}
	}

	return BT_STATUS::FAILURE;
}
BT_STATUS RandomSelector::Run()
{
	if (Composite_Node::Run() == BT_STATUS::FAILURE)
		return BT_STATUS::FAILURE;

	std::random_device rd;
	std::mt19937 gen(rd());

	vector<BTNode*> ShuffleVector;

	for (BTNode* child : m_ChildNodes)
		ShuffleVector.emplace_back(child);

	std::shuffle(ShuffleVector.begin(), ShuffleVector.end(), gen);

	for (BTNode* child : ShuffleVector)
	{
		BT_STATUS res = child->Run();

		if (res == BT_STATUS::SUCCESS)
			return BT_STATUS::SUCCESS;

		if (res == BT_STATUS::RUNNING)
		{
			((Root_Node*)GetBehaviorTree()->GetRootNode())->SetRunningNode(child);
			return BT_STATUS::SUCCESS;
		}
	}

	return BT_STATUS::FAILURE;

}
BT_STATUS Sequence::Run()
{
	if (Composite_Node::Run() == BT_STATUS::FAILURE)
		return BT_STATUS::FAILURE;
	
	for (BTNode* child : m_ChildNodes)
	{
		BT_STATUS res = child->Run();

		if (res == BT_STATUS::FAILURE)
			return BT_STATUS::FAILURE;

		if (res == BT_STATUS::RUNNING)
		{
			((Root_Node*)GetBehaviorTree()->GetRootNode())->SetRunningNode(child);
			return BT_STATUS::SUCCESS;
		}
	}

	return BT_STATUS::SUCCESS;
}

BT_STATUS Composite_Node::Run()
{
	if (m_AttachedNode != nullptr)
		return m_AttachedNode->Run();
	else
		return BT_STATUS::NONE;
}
BTNode* Composite_Node::NodeAttach(BTNode* pNode)
{
	if (m_AttachedNode)
	{
		delete m_AttachedNode;
	}

	m_AttachedNode = pNode;
	return m_AttachedNode;
}

BTNode* Task_Node::NodeAttach(BTNode* pNode)
{
	if (m_AttachedNode)
	{
		delete m_AttachedNode;
	}

	m_AttachedNode = pNode;
	return m_AttachedNode;
}

BT_STATUS Task_Node::Run()
{
	if (m_AttachedNode != nullptr)
		return m_AttachedNode->Run();
	else
		return BT_STATUS::NONE;
}

void Root_Node::AddChildNode(BTNode* pNode)
{
	assert(m_ChildNode == nullptr);

	m_ChildNode = pNode;
}

#include "pch.h"
#include "CBehaviorTree.h"

CBehaviorTree::CBehaviorTree()
	: CComponent(COMPONENT_TYPE::BEHAVIORTREE)
	, m_RootNode(nullptr)
{
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
	//case NODETYPE::BLACKBOARD:
	//	break;
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

void CBehaviorTree::AddChildNode(BTNode* ParentNode, NODETYPE eType)
{
	if (dynamic_cast<Root_Node*>(ParentNode))
	{

	}
	else if (dynamic_cast<Composite_Node*>(ParentNode))
	{

	}
	else if (dynamic_cast<Decorator_Node*>(ParentNode))
	{

	}
	else if (dynamic_cast<Service_Node*>(ParentNode))
	{

	}
	else if (dynamic_cast<Task_Node*>(ParentNode))
	{

	}
}

void CBehaviorTree::NodeAttach(BTNode* TargetNode, NODETYPE eType)
{

	if (dynamic_cast<Composite_Node*>(TargetNode))
	{
		
	}
	else if (dynamic_cast<Task_Node*>(TargetNode))
	{

	}
}

void CBehaviorTree::SetNodeName(const wstring& name)
{
}

void BTNode::SetRunningNode()
{
	m_OwnerBT->m_RootNode->SetRunningNode(this);
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
			GetBehaviorTree()->SetRunningNode(child);
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
			GetBehaviorTree()->SetRunningNode(child);
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
			GetBehaviorTree()->SetRunningNode(child);
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
void Composite_Node::NodeAttach(Decorator_Node* pNode)
{
	if (m_AttachedNode)
	{
		delete m_AttachedNode;
	}

	m_AttachedNode = pNode;
}
BT_STATUS Task_Node::Run()
{
	if (m_AttachedNode != nullptr)
		return m_AttachedNode->Run();
	else
		return BT_STATUS::NONE;
}

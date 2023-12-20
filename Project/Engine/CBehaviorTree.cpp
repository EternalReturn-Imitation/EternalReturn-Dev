#include "pch.h"
#include "CBehaviorTree.h"
#include "CBehaviorTreeMgr.h"

#pragma region BB

#pragma endregion

#pragma region BT_NODE
BTNode::BTNode(NODETYPE eType)
	: m_NodeType(eType)
	, m_RootNode(nullptr)
	, m_NodeFlag(0)
	, m_Parent(nullptr)
	, m_ChildCnt(0)
{
}

BTNode::~BTNode()
{
	for (const auto& node : m_Child)
	{
		delete node;
	}
}

void BTNode::SwapFront()
{
	if (!m_Parent 
		|| m_Parent->m_ChildCnt <= 1
		|| m_Parent->m_Child.front() == this)
		return;

	list<BTNode*>::iterator iter = m_Parent->m_Child.begin();

	while (iter != m_Parent->m_Child.end())
	{
		if (*iter == this)
		{
			iter = m_Parent->m_Child.erase(iter);
			iter--;

			m_Parent->m_Child.insert(iter, this);
			return;
		}
		iter++;
	}

	// 순회가 끝날동안 부모의 자식리스트중 자신을 찾지못했다.
	assert(nullptr);
}
void BTNode::SwapBack()
{
	if (!m_Parent
		|| m_Parent->m_ChildCnt <= 1
		|| m_Parent->m_Child.back() == this)
		return;

	list<BTNode*>::iterator iter = m_Parent->m_Child.begin();

	while (iter != m_Parent->m_Child.end())
	{
		if (*iter == this)
		{
			iter = m_Parent->m_Child.erase(iter);
			iter++;

			m_Parent->m_Child.insert(iter, this);
			return;
		}
		iter++;
	}

	// 순회가 끝날동안 부모의 자식리스트중 자신을 찾지못했다.
	assert(nullptr);
}

#pragma endregion

#pragma region Nodes
BTNode::BT_STATUS Root_Node::Run()
{
	if (m_RunningNode != nullptr)
	{
		if (m_RunningNode->Run() == BT_RUNNING)
			return BT_RUNNING;

		m_RunningNode = nullptr;

		return BT_SUCCESS;
	}
	else if (GetChildCnt() != 0)
		return m_Child.front()->Run();

	return BT_STATUS::NONE;
}
BTNode::BT_STATUS Composite_Node::Run()
{
	switch (m_NodeFlag)
	{
	case CompositeNodeFlag_SEQUENCE: 
	{
		if (Composite_Node::Run() == BT_FAILURE)
			return BT_FAILURE;

		for (BTNode* child : m_Child)
		{
			BT_STATUS res = child->Run();

			if (res == BT_FAILURE)
				return BT_FAILURE;

			if (res == BT_RUNNING)
			{
				((Root_Node*)m_RootNode)->SetRunningNode(child);
				return BT_SUCCESS;
			}
		}

		return BT_SUCCESS;
		break; 
	}
	case CompositeNodeFlag_SELECTOR:
	{
		if (Composite_Node::Run() == BT_FAILURE)
			return BT_FAILURE;

		for (BTNode* child : m_Child)
		{
			BT_STATUS res = child->Run();

			if (res == BT_SUCCESS)
				return BT_SUCCESS;

			if (res == BT_RUNNING)
			{
				((Root_Node*)m_RootNode)->SetRunningNode(child);
				return BT_SUCCESS;
			}
		}

		return BT_FAILURE;
		break;
	}
	case CompositeNodeFlag_RANDOM_SELECTOR: 
	{
		if (Composite_Node::Run() == BT_FAILURE)
			return BT_FAILURE;

		std::random_device rd;
		std::mt19937 gen(rd());

		vector<BTNode*> ShuffleVector;

		for (BTNode* child : m_Child)
			ShuffleVector.emplace_back(child);

		std::shuffle(ShuffleVector.begin(), ShuffleVector.end(), gen);

		for (BTNode* child : ShuffleVector)
		{
			BT_STATUS res = child->Run();

			if (res == BT_SUCCESS)
				return BT_SUCCESS;

			if (res == BT_RUNNING)
			{
				((Root_Node*)m_RootNode)->SetRunningNode(child);
				return BT_SUCCESS;
			}
		}

		return BT_FAILURE;
		break; 
	}
	default:
		assert(nullptr);
		break;
	}

	return BT_STATUS::NONE;
}
BTNode::BT_STATUS Decorator_Node::Run()
{
	return BTNode::BT_STATUS::NONE;
}
BTNode::BT_STATUS Task_Node::Run()
{
	return BT_STATUS::NONE;
}


#pragma endregion

#pragma region BehaviorTree
CBehaviorTree::CBehaviorTree()
	: CComponent(COMPONENT_TYPE::BEHAVIORTREE)
	, m_RootNode(nullptr)
{
}

CBehaviorTree::~CBehaviorTree()
{
	DELETE_UNVAILUBLE(m_RootNode);
}

void CBehaviorTree::tick()
{
	if(m_RootNode)
		m_RootNode->Run();
}
#pragma endregion

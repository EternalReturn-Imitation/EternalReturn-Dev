#include "pch.h"
#include "CBehaviorTree.h"

#pragma region BB

#pragma endregion

#pragma region BT_NODE
BTNode::~BTNode()
	{
		for (const auto& node : m_Child)
		{
			delete node;
		}
	}
bool BTNode::IsAncestor(BTNode* _Node)
	{
		BTNode* pParent = m_Parent;
		while (pParent)
		{
			if (pParent == _Node)
			{
				return true;
			}
			pParent = pParent->m_Parent;
		}

		return false;
	}
void BTNode::DisconnectFromParent()
	{
		if (!m_Parent)
			return;

		list<BTNode*>::iterator iter = m_Parent->m_Child.begin();
		for (; iter != m_Parent->m_Child.end(); ++iter)
		{
			if (this == *iter)
			{
				m_Parent->m_Child.erase(iter);
				m_Parent = nullptr;
				return;
			}
		}

		assert(nullptr);
	}
#pragma endregion

#pragma region Nodes
BT_STATUS Root_Node::Run()
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
BT_STATUS Composite_Node::Run()
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
				m_RootNode->SetRunningNode(child);
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
				m_RootNode->SetRunningNode(child);
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
				m_RootNode->SetRunningNode(child);
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

void Root_Node::AddChild(BTNode* ChildNode)
{
	// 자식을 사이에 끼워넣는 방식.
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



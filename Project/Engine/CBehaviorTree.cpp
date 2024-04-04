#include "pch.h"
#include "CBehaviorTree.h"



#pragma region BB
HRESULT BB::AddBBData(const wstring& _BBkey, int _Item)
{
	unordered_map<wstring, tBBData*>::iterator iter
		= m_BBDataList.find(_BBkey);

	if (iter != m_BBDataList.end())
		return S_FALSE;

	int* NewData = new int;
	*NewData = _Item;
	
	m_BBDataList.insert(make_pair(_BBkey, new tBBData(_BBkey, L"int", (DWORD_PTR)NewData)));

	return S_OK;
}

HRESULT BB::AddBBData(const wstring& _BBkey, float _Item)
{
	unordered_map<wstring, tBBData*>::iterator iter
		= m_BBDataList.find(_BBkey);

	if (iter != m_BBDataList.end())
		return S_FALSE;

	float* NewData = new float;
	*NewData = _Item;

	m_BBDataList.insert(make_pair(_BBkey, new tBBData(_BBkey, L"float", (DWORD_PTR)NewData)));

	return S_OK;
}

HRESULT BB::AddBBData(const wstring& _BBkey, CGameObject* _ItemPtr)
{
	unordered_map<wstring, tBBData*>::iterator iter
		= m_BBDataList.find(_BBkey);

	if (iter != m_BBDataList.end())
		return S_FALSE;

	m_BBDataList.insert(make_pair(_BBkey, new tBBData(_BBkey, L"GameObject", (DWORD_PTR)_ItemPtr)));

	return S_OK;
}

HRESULT BB::AddBBData(const wstring& _BBkey, wstring _wstring)
{
	unordered_map<wstring, tBBData*>::iterator iter
		= m_BBDataList.find(_BBkey);

	if (iter != m_BBDataList.end())
		return S_FALSE;

	m_BBDataList.insert(make_pair(_BBkey, new tBBData(_BBkey, L"wstring", _wstring)));

	return S_OK;
}

HRESULT BB::FindBBData(const wstring& _BBKey, int& _Dest)
{
	unordered_map<wstring, tBBData*>::iterator iter
		= m_BBDataList.find(_BBKey);

	if (iter == m_BBDataList.end())
		return S_FALSE;

	tBBData* Data = iter->second;
	const wchar_t* type = Data->strDataType;

	if (type == L"int")
	{
		_Dest = *((int*)Data->pDataPtr);
		return S_OK;
	}

	return  S_FALSE;
}

HRESULT BB::FindBBData(const wstring& _BBKey, float& _Dest)
{
	unordered_map<wstring, tBBData*>::iterator iter
		= m_BBDataList.find(_BBKey);

	if (iter == m_BBDataList.end())
		return S_FALSE;

	tBBData* Data = iter->second;
	const wchar_t* type = Data->strDataType;

	if (type == L"float")
	{
		_Dest = *((float*)Data->pDataPtr);
		return S_OK;
	}

	return  S_FALSE;
}

HRESULT BB::FindBBData(const wstring& _BBKey, CGameObject** _Dest)
{
	unordered_map<wstring, tBBData*>::iterator iter
		= m_BBDataList.find(_BBKey);

	if (iter == m_BBDataList.end())
		return S_FALSE;

	tBBData* Data = iter->second;
	const wchar_t* type = Data->strDataType;

	if (type == L"GameObject")
	{
		*_Dest = (CGameObject*)(Data->pDataPtr);
		return S_OK;
	}

	return  S_FALSE;
}

HRESULT BB::FindBBData(const wstring& _BBKey, wstring& _Dest)
{
	unordered_map<wstring, tBBData*>::iterator iter
		= m_BBDataList.find(_BBKey);

	if (iter == m_BBDataList.end())
		return S_FALSE;

	tBBData* Data = iter->second;
	const wchar_t* type = Data->strDataType;

	if (type == L"wstring")
	{
		_Dest.assign(Data->strData.begin(), Data->strData.end());

		return S_OK;
	}

	return  S_FALSE;
}

HRESULT BB::SetBBData(const wstring& _BBKey, int _Src)
{
	unordered_map<wstring, tBBData*>::iterator iter
		= m_BBDataList.find(_BBKey);

	if (iter == m_BBDataList.end())
		return S_FALSE;


	tBBData* Data = iter->second;
	const wchar_t* type = Data->strDataType;

	if (type == L"int")
	{
		*(int*)(iter->second->pDataPtr) = _Src;
		return S_OK;
	}

	return S_OK;
}

HRESULT BB::SetBBData(const wstring& _BBKey, float _Src)
{
	unordered_map<wstring, tBBData*>::iterator iter
		= m_BBDataList.find(_BBKey);

	if (iter == m_BBDataList.end())
		return S_FALSE;

	tBBData* Data = iter->second;
	const wchar_t* type = Data->strDataType;

	if (type == L"float")
	{
		*(float*)(iter->second->pDataPtr) = _Src;
		return S_OK;
	}

	return S_OK;
}

HRESULT BB::SetBBData(const wstring& _BBKey, CGameObject* _Src)
{
	unordered_map<wstring, tBBData*>::iterator iter
		= m_BBDataList.find(_BBKey);

	if (iter == m_BBDataList.end())
		return S_FALSE;

	tBBData* Data = iter->second;
	const wchar_t* type = Data->strDataType;

	if (type == L"GameObject")
	{
		iter->second->pDataPtr = (DWORD_PTR)_Src;
		return S_OK;
	}


	return S_OK;
}

HRESULT BB::SetBBData(const wstring& _BBKey, const wstring& _Src)
{
	unordered_map<wstring, tBBData*>::iterator iter
		= m_BBDataList.find(_BBKey);

	if (iter == m_BBDataList.end())
		return S_FALSE;

	tBBData* Data = iter->second;
	const wchar_t* type = Data->strDataType;

	if (type == L"wstring")
	{
		iter->second->strData = _Src;
		return S_OK;
	}

	return S_OK;
}

void BB::DeleteBBData(const wstring& _BBKey)
{
	unordered_map<wstring, tBBData*>::iterator iter = m_BBDataList.find(_BBKey);

	if (iter->second->strDataType == L"int")
	{
		delete (int*)iter->second->pDataPtr;
	}

	if (iter->second->strDataType == L"float")
	{
		delete (float*)iter->second->pDataPtr;
	}

	delete iter->second;
	m_BBDataList.erase(iter);
}

#pragma endregion

#pragma region BT_NODE
BB* BTNode::GetBlackBoard()
{
	return ((Root_Node*)m_RootNode)->GetBlackBoard();
}
BTNode::BTNode()
	: m_RootNode(nullptr)
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

	return BT_STATUS::FAILURE;
}

BT_STATUS Sequence_Node::Run()
{
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
}

BT_STATUS Selector_Node::Run()
{
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
}

BT_STATUS RandSelector_Node::Run()
{
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
}
#pragma endregion

#pragma region BehaviorTree
CBehaviorTree::CBehaviorTree()
	: CComponent(COMPONENT_TYPE::BEHAVIORTREE)
	, m_RootNode(nullptr)
{
	m_RootNode = new Root_Node;
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


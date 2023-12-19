#include "pch.h"
#include "CBehaviorTree.h"

#include "CPathMgr.h"

int SaveBTNode(BTNode* _pNode, FILE* _File);
BTNode* LoadBTNode(FILE* _File, BTNode* _Parent);
BTNode* CreateBTNode(NODETYPE _type, UINT _flag, BTNode* _RootNode = nullptr);

int SaveBehaviorTree(const wstring& _BehaviorTreePath, Root_Node* _mainRoot)
{
    wstring strPath = CPathMgr::GetInst()->GetContentPath();
    strPath += _BehaviorTreePath;

    FILE* pFile = nullptr;

    _wfopen_s(&pFile, strPath.c_str(), L"wb");

    if (nullptr == pFile)
        return E_FAIL;

    // 루트 정보 저장
    
    // 블랙보드 데이터 저장

    // 노드 저장

    
    SaveBTNode(_mainRoot, pFile);

    fclose(pFile);

    return S_OK;
}

int SaveBTNode(BTNode* _pNode, FILE* _File)
{
    // === 노드 정보 ===

    // 노드 이름
    SaveWString(_pNode->GetNodeName(), _File);

    // 노드 타입
    UINT NodeType = (UINT)_pNode->GetNodeType();
    fwrite(&NodeType, sizeof(UINT), 1, _File);

    // 노드 플래그
    UINT NodeFlag = _pNode->GetNodeFlag();
    fwrite(&NodeFlag, sizeof(UINT), 1, _File);

    // 자식노드 수
    UINT ChildCnt = _pNode->GetChildCnt();
    fwrite(&ChildCnt, sizeof(UINT), 1, _File);


    // 블랙보드 정보
    if (NodeType == (UINT)NODETYPE::DECORATOR ||
        NodeType == (UINT)NODETYPE::TASK)
    {
        // BB정보 추가로 넣어줄내용 여기에 넣기.
    }
    
    // === 자식 노드 순회저장 ===
    for (auto child : _pNode->GetChild())
    {
        SaveBTNode(child, _File);
    }

    return 0;
}

Root_Node* LoadBehaviorTree(const wstring& _BehaviorTreePath)
{
    wstring strPath = CPathMgr::GetInst()->GetContentPath();
    strPath += _BehaviorTreePath;

    FILE* pFile = nullptr;

    _wfopen_s(&pFile, strPath.c_str(), L"rb");

    if (nullptr == pFile)
        return nullptr;

    // 루트 정보 저장
    Root_Node* NewRoot = new Root_Node;
    
    wstring Name;   // 노드 이름
    LoadWString(Name, pFile);
    UINT eType = 0; // 노드 타입
    fread(&eType, sizeof(UINT), 1, pFile);
    UINT iFlag;     // 노드 플래그
    fread(&iFlag, sizeof(UINT), 1, pFile);
    UINT ChildCnt;  // 자식노드 수
    fread(&ChildCnt, sizeof(UINT), 1, pFile);

    BTNode* NewNode = CreateBTNode((NODETYPE)eType, iFlag);
    
    // 블랙보드 정보 생성
    // 블랙보드 정보 넣기

    LoadBTNode(pFile, NewNode);

    fclose(pFile);

    return NewRoot;
}

BTNode* LoadBTNode(FILE* _File, BTNode* _Parent)
{
    wstring Name;   // 노드 이름
    LoadWString(Name, _File);

    UINT eType = 0; // 노드 타입
    fread(&eType, sizeof(UINT), 1, _File);
    
    UINT iFlag;     // 노드 플래그
    fread(&iFlag, sizeof(UINT), 1, _File);

    UINT ChildCnt;  // 자식노드 수
    fread(&ChildCnt, sizeof(UINT), 1, _File);

    Root_Node* Root = (Root_Node*)(_Parent->GetRootNode());

    BTNode* NewNode = CreateBTNode((NODETYPE)eType, iFlag, Root);

    // 블랙보드 정보
    if (eType == (UINT)NODETYPE::DECORATOR ||
        eType == (UINT)NODETYPE::TASK)
    {
        // BB정보 추가로 넣어줄내용 여기에 넣기.
    }

    _Parent->AddChild(NewNode);

    for (size_t i = 0; i < ChildCnt; ++i)
    {
        LoadBTNode(_File, NewNode);
    }

    return NewNode;
}


BTNode* CreateBTNode(NODETYPE _type, UINT _flag, BTNode* _RootNode)
{
    BTNode* NewNode = nullptr;
    Root_Node* Root = dynamic_cast<Root_Node*>(_RootNode);

    switch (_type)
    {
    case NODETYPE::ROOT:
        NewNode = new Root_Node;
        Root = (Root_Node*)NewNode;
        break;
    case NODETYPE::COMPOSITE:
        NewNode = new Composite_Node;
        break;
    case NODETYPE::DECORATOR:
        NewNode = new Decorator_Node;
        break;
    case NODETYPE::TASK:
        NewNode = new Task_Node;
        break;
    }

    // Root Node 세팅
    if (Root)
    {
        NewNode->SetRootNode(Root);
    }

    // Flag 세팅
    NewNode->SetNodeFlag(_flag);

    return NewNode;
}
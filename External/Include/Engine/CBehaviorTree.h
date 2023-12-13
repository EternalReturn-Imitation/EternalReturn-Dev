#pragma once
#include "CComponent.h"

class CBehaviorTree;
class Decorator_Node;
class BTNode;

enum class NODETYPE
{
    ROOT = 0,
    COMPOSITE = 1,
    DECORATOR = 2,
    SERVICE = 3,
    TASK = 4,

    // Composite
    SELECTOR = 10,
    SEQUENCE = 11,
    RANDSELECTOR = 12,

    // Decorator
    BLACKBOARD = 20,             // 특정 블랙보드 Key에 값이 설정되어있는지 확인
    COMPARE_BBENTRIES = 21,      // 두 블랙보드 키 값을 비교하여 결과의 동일 여부 판단

    // Service


    // Task
    CUSTOM_TASK = 40,            // Client에서 생성한 TASK 실행
    PLAY_ANIMATION = 41,         // 애니메이션 재생
    PLAY_SOUND = 42,             // 사운드 재생
    WAIT = 43,                   // 대기시간 설정 : 임의의 값 설정하여 사용
    WAIT_BLACKBOARD_TIME = 44,   // 대기시간 설정 : 블랙보드에 설정된 값 사용
    
                            // Transform 제어
};
enum class BT_STATUS
{
    NONE,
    SUCCESS,
    FAILURE,
    RUNNING,
};

// ========================= 블랙보드 클래스 =========================
class BB
{
private:
    unordered_map<wstring, void*> m_ExternedItem;   // 외부 데이터
    unordered_map<wstring, void*> m_CreatedItem;    // 자체 생성 데이터
    
public:
    // 외부에서 사용중인 메모리주소를 블랙보드에 가져온다.
    template<typename T>
    T* AddItem(const wstring& key, T* Item)
    {
        unordered_map<wstring, void*>::iterator iter
            = m_ExternedItem.find(key);

        if (iter != m_ExternedItem.end())
            return (T*)(iter->second);

        m_ExternedItem.insert(make_pair(key, Item));
        iter = m_ExternedItem.find(key);

        return (T*)(iter->second);
    }

    // 블랙보드 내부에 새로 메모리를 할당하여 데이터를 입력한다.
    template<typename T>
    T* AddItem(const wstring& key, T Data)
    {
        unordered_map<wstring, void*>::iterator iter
            = m_CreatedItem.find(key);

        if (iter != m_CreatedItem.end())
            return (T*)(iter->second);

        T* NewItem = new T();
        *NewItem = T;

        m_CreatedItem.insert(make_pair(key, NewItem));
        iter = m_CreatedItem.find(key);

        return (T*)(iter->second);
    }

    template<typename T>
    T* FindItem(const wstring& key)
    {
        unordered_map<wstring, void*>::iterator iter
            = m_ExternedItem.find(key);

        if (iter != m_ExternedItem.end())
            return (T*)(iter->second);

        iter = m_CreatedItem.find(key);

        if (iter != m_CreatedItem.end())
            return (T*)(iter->second);

        return nullptr;
    }

    void ClearCreatedItem()
    {
        unordered_map<wstring, void*>::iterator iter = m_CreatedItem.begin();
        
        while (iter != m_CreatedItem.end())
        {
            delete iter->second;
            iter = m_CreatedItem.erase(iter);
        }
    }

public:
    BB() {}
    ~BB() { ClearCreatedItem(); }
};

// ========================= 기본 노드 =========================
class BTNode
{
private:
    int             m_iProgressIdx; // 실행 인덱스
    wstring         m_NodeName;     // 노드 이름
    NODETYPE        m_NodeType;     // 노드 타입
    CBehaviorTree*  m_OwnerBT;      // 소유한 BehaviorTree Component

protected:
    BTNode*         m_ParentNode;   // 부모노드
    BTNode*         m_AttachedNode; // 데코레이터,서비스 노드 장착
    BTNode*         m_ChildNode;    // 단일 자식
    list<BTNode*>   m_ChildNodes;   // 복수 자식목록

public:
    virtual BT_STATUS Run() = 0;
    
    BTNode* GetChildNode() { return m_ChildNode; }
    list<BTNode*> GetChildNodes() { return m_ChildNodes; }

    void SetParentNode(BTNode* _ParentNode) { m_ParentNode = _ParentNode; }

    NODETYPE GetNodeType() { return m_NodeType; }
    
    const wstring& GetNodeName() { return m_NodeName; }
    CBehaviorTree* GetBehaviorTree() { return m_OwnerBT; }
    
    void SetNodeName(const wstring& NodeName) { m_NodeName = NodeName; }
    void SetBehaviorTree(CBehaviorTree* pBehaviorTree) { m_OwnerBT = pBehaviorTree; }

    void DisconnectFromParent();


public:
    BTNode(NODETYPE eType) : m_iProgressIdx(-1), m_NodeType(eType), m_OwnerBT(nullptr), m_ParentNode(nullptr), m_ChildNode(nullptr), m_AttachedNode(nullptr) { m_NodeName = L"NoName"; }
    virtual ~BTNode();
};

// ========================= 루트 노드 =========================
class Root_Node : public BTNode
{
private:
    BB* m_BlackBoard;
    BTNode* m_RunningNode;

public:
    virtual BT_STATUS Run() override 
    {
        if (m_RunningNode != nullptr)
        {
            if (m_RunningNode->Run() == BT_STATUS::RUNNING)
                return BT_STATUS::RUNNING;

            m_RunningNode = nullptr;

            return BT_STATUS::SUCCESS;
        }
        else if (m_ChildNode != nullptr)
            return m_ChildNode->Run();

        return BT_STATUS::NONE;
    };
    
    void AddChildNode(BTNode* pNode);
    void SetRunningNode(BTNode* pNode) { m_RunningNode = pNode; }
    BB* GetBlackBoard() { return m_BlackBoard; }

public:
    Root_Node() : BTNode(NODETYPE::ROOT), m_BlackBoard(nullptr), m_RunningNode(nullptr) { m_BlackBoard = new BB(); }
    virtual ~Root_Node() { DELETE_UNVAILUBLE(m_BlackBoard); m_RunningNode = nullptr; }
};

// ========================= 컴포짓 노드 =========================
#pragma region Composite Node
// 분기의 루트와 해당 분기가 실행되는 방식의 기본 규칙
class Composite_Node : public BTNode
{
public:
    virtual BT_STATUS Run();
    virtual BTNode* NodeAttach(BTNode* pNode);
    virtual BTNode* AddChildNode(BTNode* pNode)
    {
        m_ChildNodes.emplace_back(pNode);
        return pNode;
    }

public:
    Composite_Node() : BTNode(NODETYPE::COMPOSITE) {}
    virtual ~Composite_Node() {}
};

class Selector : public Composite_Node
{
public:
    virtual BT_STATUS Run();

public:
    Selector() { SetNodeName(L"NewSelectorNode"); }
    virtual ~Selector() {}
};

class Sequence : public Composite_Node
{
public:
    virtual BT_STATUS Run();

public:
    Sequence() { SetNodeName(L"NewSequenceNode"); }
};

class RandomSelector : public Composite_Node
{
public:
    virtual BT_STATUS Run();

public:
    RandomSelector() { SetNodeName(L"NewRandomSelectorNode"); }
};

#pragma endregion

// ========================= 데코레이터 노드 =========================
#pragma region Decorator Node
class Decorator_Node : public BTNode
{
private:


public:
    Decorator_Node() : BTNode(NODETYPE::DECORATOR) {}
    virtual ~Decorator_Node() {}
    
};
// ===============================
class Compare_BBEntries : public Decorator_Node
{
private:
    bool    IsEqualTo;
    wstring BBKeyA;
    wstring BBKeyB;

public:
    virtual BT_STATUS Run() override {}
    
public:
    Compare_BBEntries() : IsEqualTo(false) {}
    virtual ~Compare_BBEntries() {}
};

#pragma endregion

// ========================= 서비스 노드 =========================
#pragma region Service Node
// 컴포짓 노드에 어태치, 블랙보드의 확인 및 업데이트에 사용.
class Service_Node : public BTNode
{
private:


public:
    Service_Node() : BTNode(NODETYPE::SERVICE) {}
    virtual ~Service_Node() {}

};
#pragma endregion

// ========================= 테스크(리프) 노드 =========================
#pragma region Task Node
class Task_Node : public BTNode
{
public:
    virtual BT_STATUS Run();
    virtual BTNode* NodeAttach(BTNode* pNode);

public:
    Task_Node() : BTNode(NODETYPE::TASK) { SetNodeName(L"NewTaskNode"); }
    virtual ~Task_Node() {}
};
#pragma endregion


// ========================= 행동트리 컴포넌트 =========================
class CBehaviorTree :
    public CComponent
{
private:
    Root_Node* m_RootNode;

private:
    BTNode* CreateNewNode(NODETYPE eType);

public:
    virtual void begin() {}
    virtual void tick();
    virtual void finaltick();

public:
    BTNode* AddChildNode(BTNode* ParentNode, NODETYPE eType);
    BTNode* NodeAttach(BTNode* TargetNode, NODETYPE eType);
    BTNode* GetRootNode() { return m_RootNode; }

public:
    template<typename T>
    T* FindItem(const wstring& key) { return m_RootNode->GetBlackBoard()->FindItem(key); }

    // 외부 아이템 추가
    template<typename T>
    T* AddItem(const wstring& key, T* Item) { return m_RootNode->GetBlackBoard()->AddItem(key, Item); }

    // 신규 아이템 생성 및 데이터 입력
    template<typename T>
    T* AddItem(const wstring& key, T Data) { return m_RootNode->GetBlackBoard()->AddItem(key, Data); }

    // 신규 아이템 생성 (메모리 생성만 진행)
    template<typename T>
    T* AddItem(const wstring& key) { return m_RootNode->GetBlackBoard()->AddItem(key); }

public:
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}

    CLONE(CBehaviorTree);

public:
    CBehaviorTree();
    ~CBehaviorTree();

    friend class BTNode;
};


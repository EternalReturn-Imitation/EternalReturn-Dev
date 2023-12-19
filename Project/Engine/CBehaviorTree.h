#pragma once
#include "CComponent.h"

class CBehaviorTree;
class Decorator_Node;
class BTNode;

enum class NODETYPE
{
    ROOT,       // 루트노드
    COMPOSITE,  // 브랜치노드   : 복수 자식
    DECORATOR,  // 조건노드     : 단일 자식
    TASK        // 실행노드     : 자식 불가
};

enum class BT_STATUS
{
    NONE,
    SUCCESS,
    FAILURE,
    RUNNING,
};

enum class BBType
{
    INT,
    FLOAT,
    BOOL,
    GAMEOBJECT,
    STRING,
};

struct BBkey
{
    wstring key;
    BBType  type;
};

#define BT_SUCCESS BT_STATUS::SUCCESS
#define BT_FAILURE BT_STATUS::FAILURE
#define BT_RUNNING BT_STATUS::RUNNING

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
protected:
    wstring         m_NodeName;     // 노드 이름
    NODETYPE        m_NodeType;     // 노드 타입 : ROOT,COMPOSITE,DECORATOR,TASK
    Root_Node*      m_RootNode;     // Root_Node
    int             m_NodeFlag;     // FlagType
    
    BTNode*         m_Parent;       // 부모노드
    list<BTNode*>   m_Child;        // 자식노드 리스트
    
    UINT            m_ChildCnt;     // 자식노드 수

public:
    virtual BT_STATUS Run() = 0;
    
    // ========= 부모 노드 =========
    BTNode* GetParentNode() { return m_Parent; }
    void SetParentNode(BTNode* _ParentNode) { m_Parent = _ParentNode; }

    // ========= 자식 노드 =========
    virtual void AddChild(BTNode* ChildNode) {};
    UINT GetChildCnt() { return m_ChildCnt; }

    // ========= 노드 정보 =========
    const wstring& GetNodeName() { return m_NodeName; }
    void SetNodeName(const wstring& NodeName) { m_NodeName = NodeName; }
    
    NODETYPE GetNodeType() { return m_NodeType; }
    
    Root_Node* GetRootNode() { return m_RootNode; }
    void SetRootNode(Root_Node* _RootNode) { m_RootNode = _RootNode; }

    const int GetNodeFlag() { return m_NodeFlag; }
    void SetNodeFlag(UINT _flag) { m_NodeFlag = _flag; }

    void DisconnectFromParent();
    bool IsAncestor(BTNode* _Node);

public:
    BTNode(NODETYPE eType) {}
    virtual ~BTNode();
};

// ========================= 루트 노드 =========================
class Root_Node : public BTNode
{
private:
    BB* m_BlackBoard;
    BTNode* m_RunningNode;

public:
    virtual BT_STATUS Run() override;
    
    virtual void AddChild(BTNode* ChildNode);

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
    enum CompositNodeFlag
    {
        CompositeNodeFlag_SEQUENCE,
        CompositeNodeFlag_SELECTOR,
        CompositeNodeFlag_RANDOM_SELECTOR,
    };

public:
    virtual BT_STATUS Run();
    virtual void AddChild(BTNode* ChildNode) {};

public:
    Composite_Node() : BTNode(NODETYPE::COMPOSITE) {}
    virtual ~Composite_Node() {}
};

#pragma endregion

// ========================= 데코레이터 노드 =========================
#pragma region Decorator Node
class Decorator_Node : public BTNode
{
public:
    enum DecoratorNodeFlag
    {
        DecoratorNodeFlag_BLACKBOARD,           // 특정 블랙보드 Key에 값이 설정되어있는지 확인
        DecoratorNodeFlag_COMPARE_BBENTRIES,    // 두 블랙보드 키 값을 비교하여 결과의 동일 여부 판단
    };

public:
    virtual BT_STATUS Run();

    Decorator_Node() : BTNode(NODETYPE::DECORATOR) {}
    virtual ~Decorator_Node() {}
    
};
#pragma endregion

// ========================= 테스크(리프) 노드 =========================
#pragma region Task Node
class Task_Node : public BTNode
{
public:
    enum TaskNodeFlag
    {
        TaskNodeFlag_PLAY_ANIMATION,         // 애니메이션 재생
        TaskNodeFlag_PLAY_SOUND,             // 사운드 재생
        TaskNodeFlag_WAIT,                   // 대기시간 설정 : 임의의 값 설정하여 사용
        TaskNodeFlag_WAIT_BLACKBOARD_TIME,   // 대기시간 설정 : 블랙보드에 설정된 값 사용
    };
    
    virtual BT_STATUS Run();

public:
    Task_Node() : BTNode(NODETYPE::TASK) {}
    virtual ~Task_Node() {}
};
#pragma endregion

BTNode* CreateBTNode(NODETYPE _type, int _flag = -1)
{
    BTNode* NewNode = nullptr;

    switch (_type)
    {
    case NODETYPE::ROOT:
        NewNode = new Root_Node;
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

    // Flag 정보가 있음
    if (0 <= _flag)
    {
        NewNode->SetNodeFlag(_flag);
    }

    return NewNode;
}

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
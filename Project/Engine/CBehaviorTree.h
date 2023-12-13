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

class BTNode
{
private:
    int         m_iProgressIdx; // 실행 인덱스
    wstring     m_NodeName;     // 노드 이름
    NODETYPE    m_NodeType;     // 노드 타입
    BTNode*     m_ParentNode;   // 부모노드
    CBehaviorTree* m_OwnerBT;   // 소유한 BehaviorTree Component

private:
    void SetRunningNode(); 

protected:
    BTNode* GetParentNode() { return m_ParentNode; }
    void SetParentNode(BTNode* pNode) { m_ParentNode = pNode; }

public:
    NODETYPE GetNodeType() { return m_NodeType; }
    void SetNodeName(const wstring& NodeName) { m_NodeName = NodeName; }

    virtual BT_STATUS Run() = 0;
    void SetOwnerTree(CBehaviorTree* OwnerTree) { m_OwnerBT = OwnerTree; }
    void SetBehaviorTree(CBehaviorTree* pBehaviorTree) { m_OwnerBT = pBehaviorTree; }
    CBehaviorTree* GetBehaviorTree() { return m_OwnerBT; }


public:
    BTNode(NODETYPE eType) : m_iProgressIdx(-1), m_NodeType(eType), m_ParentNode(nullptr), m_OwnerBT(nullptr) { m_NodeName = L"NoName"; }
    ~BTNode() {}
};
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
class Root_Node : public BTNode
{
private:
    BB* m_BlackBoard;
    BTNode* m_ChildNode;
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
    
    void SetRunningNode(BTNode* pNode) { m_RunningNode = pNode; }
    BB* GetBlackBoard() { return m_BlackBoard; }

public:
    Root_Node() : BTNode(NODETYPE::ROOT), m_BlackBoard(nullptr), m_ChildNode(nullptr), m_RunningNode(nullptr) { m_BlackBoard = new BB(); }
    ~Root_Node() { DELETE_UNVAILUBLE(m_BlackBoard); DELETE_UNVAILUBLE(m_ChildNode); m_RunningNode = nullptr; }
};
#pragma region Composite Node
// 분기의 루트와 해당 분기가 실행되는 방식의 기본 규칙
class Composite_Node : public BTNode
{
protected:
    Decorator_Node* m_AttachedNode; // 데코레이터 노드 장착
    list<BTNode*> m_ChildNodes;     // 자식노드

public:
    virtual BT_STATUS Run();
    virtual void NodeAttach(Decorator_Node* pNode);
    virtual void AddChildNode(BTNode* pNode)
    {
        m_ChildNodes.emplace_back(pNode);
    }

public:
    Composite_Node() : BTNode(NODETYPE::COMPOSITE), m_AttachedNode(nullptr) {}
    ~Composite_Node() {}
};

class Selector : public Composite_Node
{
public:
    virtual BT_STATUS Run();
};

class Sequence : public Composite_Node
{
public:
    virtual BT_STATUS Run();
};

class RandomSelector : public Composite_Node
{
public:
    virtual BT_STATUS Run();
};

#pragma endregion
#pragma region Decorator Node
// 조건식
class Decorator_Node : public BTNode
{
private:


public:
    Decorator_Node() : BTNode(NODETYPE::DECORATOR) {}
    ~Decorator_Node() {}
    
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
    ~Compare_BBEntries() {}
};

#pragma endregion
#pragma region Service Node
// 컴포짓 노드에 어태치, 블랙보드의 확인 및 업데이트에 사용.
class Service_Node : public BTNode
{
private:


public:
    Service_Node() : BTNode(NODETYPE::SERVICE) {}
    ~Service_Node() {}

};
#pragma endregion
#pragma region Task Node
// 리프노드
class Task_Node : public BTNode
{
protected:
    Decorator_Node* m_AttachedNode; // 데코레이터 노드 장착

public:
    virtual BT_STATUS Run();

public:
    Task_Node() : BTNode(NODETYPE::TASK), m_AttachedNode(nullptr) {}
    ~Task_Node() {}
};
#pragma endregion

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
    void SetRunningNode(BTNode* pNode) { m_RootNode->SetRunningNode(pNode); }
    void AddChildNode(BTNode* ParentNode, NODETYPE eType);
    void NodeAttach(BTNode* TargetNode ,NODETYPE eType);
    void SetNodeName(const wstring& name);


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


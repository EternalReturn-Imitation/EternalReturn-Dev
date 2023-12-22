#pragma once
#include "CComponent.h"

#define BT_SUCCESS BTNode::BT_STATUS::SUCCESS
#define BT_FAILURE BTNode::BT_STATUS::FAILURE
#define BT_RUNNING BTNode::BT_STATUS::RUNNING
#define BT_ROOT BTNode::NODETYPE::ROOT
#define BT_COMPOSITE BTNode::NODETYPE::COMPOSITE
#define BT_DECORATOR BTNode::NODETYPE::DECORATOR
#define BT_TASK BTNode::NODETYPE::TASK

class CBehaviorTree;
class Decorator_Node;
class BTNode;

struct tBBKey
{
    enum eDataType
    {
        UNKNOWN,
        INT,
        FLOAT,
        VECTOR2,
        VECTOR3,
        VECTOR4,
    };

    enum eDataSlot
    {
        EXTERNED,
        CREATED,
    };

    string Key;
    eDataType Type = eDataType::UNKNOWN;
    eDataSlot Slot = eDataSlot::CREATED;

    void GetBBKeyInfoString(string& _Type, string& _Slot)
    {
        _Type = GetType();
        _Slot = GetSlot();
    }

    const char* GetType()
    {
        const char* type = nullptr;

        switch (Type)
        {
        case tBBKey::UNKNOWN:
            type = "Unknown";
            break;
        case tBBKey::INT:
            type = "int";
            break;
        case tBBKey::FLOAT:
            type = "float";
            break;
        case tBBKey::VECTOR2:
            type = "Vec2";
            break;
        case tBBKey::VECTOR3:
            type = "Vec3";
            break;
        case tBBKey::VECTOR4:
            type = "Vec4";
            break;
        }

        return type;
    }

    const char* GetSlot()
    {
        const char* slot= nullptr;

        switch (Slot)
        {
        case tBBKey::EXTERNED:
            slot = "Externed";
            break;
        case tBBKey::CREATED:
            slot = "Created";
            break;
        default:
            break;
        }
        return slot;
    }
    
};

// ========================= 블랙보드 클래스 =========================
class BB
{
private:
    unordered_map<string, void*> m_ExternedItem;   // 외부 데이터
    unordered_map<string, void*> m_CreatedItem;    // 자체 생성 데이터
    list<tBBKey> m_DataKeyList;                    // 보유중인 데이터 리스트

    // ExternedItem 은 Load 했을때 가져올 수 있도록
    // GameObject에서 얻어오는 함수나, System에서 가져올 수 있는 함수의
    // 인자를 저장한다.

private:
    template<typename T>
    void AddBBKey(string _key, bool _IsExterned)
    {
        tBBKey tmp;
        tmp.Key = _key;

        string type = typeid(T).name();

        if (type == typeid(int).name())
        {
            tmp.Type = tBBKey::eDataType::INT;
        }
        else if (type == typeid(float).name())
        {
            tmp.Type = tBBKey::eDataType::FLOAT;
        }
        else if (type == typeid(Vector2).name())
        {
            tmp.Type = tBBKey::eDataType::VECTOR2;
        }
        else if (type == typeid(Vector3).name())
        {
            tmp.Type = tBBKey::eDataType::VECTOR3;
        }
        else if (type == typeid(Vector4).name())
        {
            tmp.Type = tBBKey::eDataType::VECTOR4;
        }
        else
        {
            tmp.Type = tBBKey::eDataType::UNKNOWN;
        }

        if (_IsExterned)
        {
            tmp.Slot = tBBKey::eDataSlot::EXTERNED;
            
        }
        else
        {
            tmp.Slot = tBBKey::eDataSlot::CREATED;
        }
        
        m_DataKeyList.push_back(tmp);
    }

public:
    // 외부에서 사용중인 메모리주소를 블랙보드에 가져온다.
    template<typename T>
    T* AddItem(const string& key, T* ItemAdress)
    {
        unordered_map<string, void*>::iterator iter
            = m_ExternedItem.find(key);

        if (iter != m_ExternedItem.end())
            return (T*)(iter->second);

        m_ExternedItem.insert(make_pair(key, ItemAdress));
        iter = m_ExternedItem.find(key);

        AddBBKey<T>(key, true);

        return (T*)(iter->second);
    }

    // 블랙보드 내부에 새로 메모리를 할당하여 데이터를 입력한다.
    template<typename T>
    T* AddItem(const string& key, T Data)
    {
        unordered_map<string, void*>::iterator iter
            = m_CreatedItem.find(key);

        if (iter != m_CreatedItem.end())
            return (T*)(iter->second);

        T* NewItem = new T();
        *NewItem = Data;

        m_CreatedItem.insert(make_pair(key, NewItem));
        iter = m_CreatedItem.find(key);

        AddBBKey<T>(key, false);

        return (T*)(iter->second);
    }

    template<typename T>
    T* AddItem(const string& key)
    {
        unordered_map<string, void*>::iterator iter
            = m_CreatedItem.find(key);

        if (iter != m_CreatedItem.end())
            return (T*)(iter->second);

        T* NewItem = new T();
        *NewItem = 0;

        m_CreatedItem.insert(make_pair(key, NewItem));
        iter = m_CreatedItem.find(key);

        AddBBKey<T>(key, false);

        return (T*)(iter->second);
    }

    template<typename T>
    T* FindItem(const string& key)
    {
        unordered_map<string, void*>::iterator iter
            = m_ExternedItem.find(key);

        if (iter != m_ExternedItem.end())
            return (T*)(iter->second);

        iter = m_CreatedItem.find(key);

        if (iter != m_CreatedItem.end())
            return (T*)(iter->second);

        return nullptr;
    }

    void GetvalueStr(tBBKey _key, string& _Dest)
    {
        char tmp[100] = {};

        switch (_key.Type)
        {
        case tBBKey::UNKNOWN:
            _Dest = "--";
            break;
        case tBBKey::INT:
        {
            int* data = FindItem<int>(_key.Key);
            sprintf_s(tmp, "%d", *data);
            break;
        }
        case tBBKey::FLOAT:
        {
            float* data = FindItem<float>(_key.Key);
            sprintf_s(tmp, "%.3f", *data);
            break;
        }
        case tBBKey::VECTOR2:
        {
            Vector2* data = FindItem<Vector2>(_key.Key);
            sprintf_s(tmp, "{ %.3f, %.3f}", (*data).x, (*data).y);
            break;
        }
        case tBBKey::VECTOR3:
        {
            Vector3* data = FindItem<Vector3>(_key.Key);
            sprintf_s(tmp, "{ %.3f, %.3f, %.3f}", (*data).x, (*data).y, (*data).z);
            break;
        }
        case tBBKey::VECTOR4:
        {
            Vector4* data = FindItem<Vector4>(_key.Key);
            sprintf_s(tmp, "{ %.3f, %.3f, %.3f, %.3f}", (*data).x, (*data).y, (*data).z, (*data).w);
        }
        break;
        }

        _Dest = tmp;
    }

    void ClearCreatedItem()
    {
        unordered_map<string, void*>::iterator iter = m_CreatedItem.begin();
        
        while (iter != m_CreatedItem.end())
        {
            delete iter->second;
            iter = m_CreatedItem.erase(iter);
        }
    }

    const list<tBBKey>& GetKeyList() { return m_DataKeyList; }

public:
    BB() { }
    ~BB() { ClearCreatedItem(); }
};

// ========================= 기본 노드 =========================
class BTNode
{
public:
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

protected:
    wstring         m_NodeName;     // 노드 이름
    NODETYPE        m_NodeType;     // 노드 타입 : ROOT,COMPOSITE,DECORATOR,TASK
    BTNode*         m_RootNode;     // Root_Node
    UINT            m_NodeFlag;     // FlagType
    
    BTNode*         m_Parent;       // 부모노드
    list<BTNode*>   m_Child;        // 자식노드 리스트
    
    UINT            m_ChildCnt;     // 자식노드 수

public:
    virtual BT_STATUS Run() { return BT_STATUS::NONE; }

    // ========= 노드 관계 =========
   
    void SwapFront();
    void SwapBack();

    BTNode* DisconnectFromParent()
    {
        if (!m_Parent)
            return nullptr;

        BTNode* BeforeParentNode = m_Parent;

        list<BTNode*>::iterator iter = m_Parent->m_Child.begin();

        for (; iter != m_Parent->m_Child.end(); ++iter)
        {
            if (this == *iter)
            {
                m_Parent->m_Child.erase(iter);
                m_Parent->m_ChildCnt--;
                m_Parent = nullptr;

                return BeforeParentNode;
            }
        }

        assert(nullptr);

        return nullptr;
    }

    bool IsAncestorNode(BTNode* _Node)
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

    // ========= 부모 노드 =========
    BTNode* GetParentNode() { return m_Parent; }
    void SetParentNode(BTNode* _ParentNode) { m_Parent = _ParentNode; }


    // ========= 자식 노드 =========
    BTNode* AddChild(BTNode* ChildNode)
    {
        // Task Node라면 자식을 가질 수 없음.
        if (m_NodeType == NODETYPE::TASK)
        {
            return ChildNode;
            assert(nullptr);
        }

        // 자식이 1개 이상 있을 경우
        if (0 < m_ChildCnt)
        {
            // 단일자식을 가지는 노드
            if (m_NodeType == NODETYPE::ROOT
                || m_NodeType == NODETYPE::DECORATOR)
            {
                // 기존 자식과 부모 관계끊기
                BTNode* BeforeChild = m_Child.front();

                ChildNode->AddChild(BeforeChild);

                
                ChildNode->DisconnectFromParent();
                ChildNode->SetRootNode(m_RootNode);
                m_Child.emplace_back(ChildNode);
                ChildNode->SetParentNode(this);
                m_ChildCnt++;

                return ChildNode;
            }
        }
        
        ChildNode->DisconnectFromParent();
        ChildNode->SetRootNode(m_RootNode);
        m_Child.emplace_back(ChildNode);
        ChildNode->SetParentNode(this);
        m_ChildCnt++;

        return ChildNode;
    }

    UINT GetChildCnt() { return m_ChildCnt; }
    list<BTNode*> GetChild() { return m_Child; }


    // ========= 노드 정보 =========
    const wstring& GetNodeName() { return m_NodeName; }
    void SetNodeName(const wstring& NodeName) { m_NodeName = NodeName; }

    
    NODETYPE GetNodeType() { return m_NodeType; }
    
    BTNode* GetRootNode() { return m_RootNode; }
    void SetRootNode(BTNode* _RootNode) { m_RootNode = _RootNode; }

    int GetNodeFlag() { return m_NodeFlag; }
    void SetNodeFlag(UINT _flag) { m_NodeFlag = _flag; }
    virtual const char** GetFlagList() { return nullptr; }
    virtual UINT GetFlagCnt() { return 0; }

    

public:
    BTNode(NODETYPE eType);
    virtual ~BTNode();

    
};

// ========================= 루트 노드 =========================
class Root_Node : public BTNode
{
private:
    CGameObject* m_OwnerObj;
    BB* m_BlackBoard;
    BTNode* m_RunningNode;

public:
    virtual BT_STATUS Run() override;
    
    // ========= 블랙 보드 =========
    template<typename T>
    T* FindItem(const wstring& key) { return m_BlackBoard->FindItem(key); }

    // 외부 아이템 추가
    template<typename T>
    T* AddItem(const wstring& key, T* Item) { return m_BlackBoard->AddItem(key, Item); }

    // 신규 아이템 생성 및 데이터 입력
    template<typename T>
    T* AddItem(const wstring& key, T Data) { return m_BlackBoard->AddItem(key, Data); }

    // 신규 아이템 생성 (메모리 생성만 진행)
    template<typename T>
    T* AddItem(const wstring& key) { return m_BlackBoard->AddItem(key); }

    void SetRunningNode(BTNode* pNode) { m_RunningNode = pNode; }
    BB* GetBlackBoard() { return m_BlackBoard; }

public:
    Root_Node() : BTNode(NODETYPE::ROOT), m_BlackBoard(nullptr), m_RunningNode(nullptr), m_OwnerObj(nullptr) { m_BlackBoard = new BB(); SetNodeName(L"NewRoot"); }
    virtual ~Root_Node() { DELETE_UNVAILUBLE(m_BlackBoard); m_RunningNode = nullptr; }

    friend class CBehaviorTree;
    friend class Decorator_Node;
    friend class Task_Node;
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
        END,
    };

public:
    virtual BT_STATUS Run();
    virtual UINT GetFlagCnt() { return CompositNodeFlag::END; }
    virtual const char** GetFlagList()
    { 
        static const char* CompositeFlags[] =
        {
            "SEQUENCE",
            "SELECTOR",
            "RANDOM_SELECTOR"
        };

        return CompositeFlags;
    }

public:
    Composite_Node() : BTNode(NODETYPE::COMPOSITE) { SetNodeName(L"NewComposite"); }
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
        END,
    };

public:
    virtual BT_STATUS Run();
    virtual UINT GetFlagCnt() { return DecoratorNodeFlag::END; }
    virtual const char** GetFlagList()
    {
        static const char* DecoratorFlags[] =
        {
            "BLACKBOARD",
            "COMPARE_BBENTRIES"
        };

        return DecoratorFlags;
    }

    Decorator_Node() : BTNode(NODETYPE::DECORATOR) { SetNodeName(L"NewDecorator"); }
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
        END,
    };
    
    virtual BT_STATUS Run();
    virtual UINT GetFlagCnt() { return TaskNodeFlag::END; }
    virtual const char** GetFlagList()
    {
        static const char* TaskFlags[] =
        {
            "PLAY_ANIMATION",
            "PLAY_SOUND",
            "WAIT",
            "WAIT_BLACKBOARD_TIME"
        };

        return TaskFlags;
    }

public:
    Task_Node() : BTNode(NODETYPE::TASK) { SetNodeName(L"NewTask"); }
    virtual ~Task_Node() {}
};
#pragma endregion

// ========================= 행동트리 컴포넌트 =========================
class CBehaviorTree :
    public CComponent
{
private:
    Root_Node* m_RootNode;

public:
    virtual void begin() {}
    virtual void tick();
    virtual void finaltick() {}

public:
    BTNode* GetRootNode() { return m_RootNode; }

public:
    Root_Node* SetRootNode(Root_Node* _Root)
    { 
        m_RootNode = _Root;
        m_RootNode->m_OwnerObj = GetOwner();

        return m_RootNode;
    }

public:
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}

    CLONE(CBehaviorTree);

public:
    CBehaviorTree();
    ~CBehaviorTree();

    friend class BTNode;
};
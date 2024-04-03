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

// ========================= 블랙보드 클래스 =========================
class BB
{
public:
    struct tBBData
    {
        string      strKey;
        const char* strDataType;
        DWORD_PTR   pDataPtr;
        string      strData;
        
        tBBData(string _key, const char* _Type, DWORD_PTR _Dataptr) : strKey(_key), strDataType(_Type), pDataPtr(_Dataptr) {}
        tBBData(string _key, const char* _Type, string _strData) : strKey(_key), strDataType(_Type), pDataPtr(0), strData(_strData) {}
    };

private:
    unordered_map<string, tBBData*> m_BBDataList;    // Total BB List
 
public:
    HRESULT AddBBData(const string& _BBKey, int _ItemPtr);
    HRESULT AddBBData(const string& _BBKey, float _ItemPtr);
    HRESULT AddBBData(const string& _BBKey, CGameObject* _ItemPtr);
    HRESULT AddBBData(const string& _BBKey, string _string);
    HRESULT AddBBData(const string& _BBKey, wstring _wstring);

    unordered_map<string, tBBData*> GetBBList() { return m_BBDataList; }

    HRESULT FindBBData(const string& _BBKey, int& _Dest);
    HRESULT FindBBData(const string& _BBKey, float& _Dest);
    HRESULT FindBBData(const string& _BBKey, CGameObject* _Dest);
    HRESULT FindBBData(const string& _BBKey, string& _Dest);
    HRESULT FindBBData(const string& _BBKey, wstring& _Dest);

    void DeleteBBData(const string& _BBKey);

    void CleartBBData()
    {
        unordered_map<string, tBBData*>::iterator iter = m_BBDataList.begin();;
        
        while (iter != m_BBDataList.end())
        {
            if (iter->second->strDataType == "int")
            {
                delete (int*)iter->second->pDataPtr;
            }

            if(iter->second->strDataType == "float")
            {
                delete (float*)iter->second->pDataPtr;
            }

            delete iter->second;
            iter = m_BBDataList.erase(iter);
        }
    }

public:
    BB() { }
    ~BB() { CleartBBData(); }
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

    struct tSrcItem
    {
        int     INT[4] = {};
        float   FLOAT[4] = {};
        string  STRING;
    };

protected:
    wstring         m_NodeName;     // 노드 이름
    NODETYPE        m_NodeType;     // 노드 타입 : ROOT,COMPOSITE,DECORATOR,TASK
    BTNode*         m_RootNode;     // Root_Node
    UINT            m_NodeFlag;     // FlagType
    
    BTNode*         m_Parent;       // 부모노드
    list<BTNode*>   m_Child;        // 자식노드 리스트
    
    UINT            m_ChildCnt;     // 자식노드 수
    tSrcItem        m_SrcItem;      // 노드자체 보유 Item

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
    void SetRunningNode(BTNode* pNode) { m_RunningNode = pNode; }
    
    // ========= 블랙 보드 =========
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
        delete m_RootNode;

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
#pragma once
#include "CComponent.h"

#define BT_SUCCESS BT_STATUS::SUCCESS
#define BT_FAILURE BT_STATUS::FAILURE
#define BT_RUNNING BT_STATUS::RUNNING
#define BT_INVALID BT_STATUS::INVALID

enum class BT_STATUS
{
    SUCCESS,	// 성공
    FAILURE,	// 실패
    RUNNING,	// 진행중
    INVALID		// 유효하지 않음
};

// ========================= 블랙보드 클래스 =========================
class BB
{
public:
    struct tBBData
    {
        wstring         strKey;
        const wchar_t*  strDataType;
        DWORD_PTR       pDataPtr;
        wstring         strData;
        
        tBBData(wstring _key, const wchar_t* _Type, DWORD_PTR _Dataptr) : strKey(_key), strDataType(_Type), pDataPtr(_Dataptr) {}
        tBBData(wstring _key, const wchar_t* _Type, wstring _strData) : strKey(_key), strDataType(_Type), pDataPtr(0), strData(_strData) {}
    };

private:
    unordered_map<wstring, tBBData*> m_BBDataList;    // Total BB List
 
public:
    HRESULT AddBBData(const wstring& _BBKey, int _ItemPtr);
    HRESULT AddBBData(const wstring& _BBKey, float _ItemPtr);
    HRESULT AddBBData(const wstring& _BBKey, CGameObject* _ItemPtr);
    HRESULT AddBBData(const wstring& _BBKey, wstring _wstring);

    unordered_map<wstring, tBBData*> GetBBList() { return m_BBDataList; }

    HRESULT FindBBData(const wstring& _BBKey, int& _Dest);
    HRESULT FindBBData(const wstring& _BBKey, float& _Dest);
    HRESULT FindBBData(const wstring& _BBKey, CGameObject** _Dest);
    HRESULT FindBBData(const wstring& _BBKey, wstring& _Dest);

    HRESULT SetBBData(const wstring& _BBKey, int _Src);
    HRESULT SetBBData(const wstring& _BBKey, float _Src);
    HRESULT SetBBData(const wstring& _BBKey, CGameObject* _Src);
    HRESULT SetBBData(const wstring& _BBKey, const wstring& _Src);

    void DeleteBBData(const wstring& _BBKey);

    void ClearBBData()
    {
        unordered_map<wstring, tBBData*>::iterator iter = m_BBDataList.begin();;
        
        while (iter != m_BBDataList.end())
        {
            if (iter->second->strDataType == L"int")
            {
                delete (int*)iter->second->pDataPtr;
            }

            if(iter->second->strDataType == L"float")
            {
                delete (float*)iter->second->pDataPtr;
            }

            delete iter->second;
            iter = m_BBDataList.erase(iter);
        }
    }

public:
    BB() { }
    ~BB() { ClearBBData(); }
};

class Root_Node;

// ========================= 기본 노드 =========================
class BTNode
{
protected:
    wstring         m_NodeName;     // 노드 이름
    BTNode*         m_RootNode;     // Root_Node
    
    BTNode*         m_Parent;       // 부모노드
    list<BTNode*>   m_Child;        // 자식노드 리스트
    
    UINT            m_ChildCnt;     // 자식노드 수

    CGameObject*    m_pOwner;

public:
    virtual BT_STATUS Run() = 0;

    // ========= 부모 노드 =========
    BTNode* GetParentNode() { return m_Parent; }
    void SetParentNode(BTNode* _ParentNode) { m_Parent = _ParentNode; }

    // ========= 자식 노드 =========
    template<typename T>
    T* AddChild()
    {
        T* ChildNode = new T();

        // 생성하는 객체가 BTNode 상속이 아니면 생성불가
        if (nullptr == dynamic_cast<BTNode*>(ChildNode))
        {
            delete ChildNode;
            return nullptr;
        }
                
        ChildNode->SetRootNode(m_RootNode);     // 루트 노드 설정
        m_Child.emplace_back(ChildNode);        // 자식 노드로 추가
        ChildNode->SetParentNode(this);         // 부모노드 설정
        ChildNode->m_pOwner = m_pOwner;         // 게임오브젝트 등록
        m_ChildCnt++;                           // 자식갯수 추가

        return ChildNode;
    }

    UINT GetChildCnt() { return m_ChildCnt; }
    list<BTNode*> GetChild() { return m_Child; }

    // ========= 노드 정보 =========
    const wstring&          GetNodeName() { return m_NodeName; }
    void                    SetNodeName(const wstring& NodeName) { m_NodeName = NodeName; }
    
    BTNode*                 GetRootNode() { return m_RootNode; }
    void                    SetRootNode(BTNode* _RootNode) { m_RootNode = _RootNode; }

    BB*                     GetBlackBoard();

    CGameObject*            GetOwner() { return m_pOwner; }

public:
    BTNode();
    virtual ~BTNode();

    friend class Root_Node;
};

// ========================= 루트 노드 =========================
class Root_Node : public BTNode
{
protected:
    BB*                 m_BlackBoard;
    BTNode*             m_RunningNode;
    

public:
    virtual BT_STATUS Run() override;
    void SetRunningNode(BTNode* pNode) { m_RunningNode = pNode; }
    void SetOwner(CGameObject* _Owner) { m_pOwner = _Owner; }

    // ========= 자식 노드 =========
    template<typename T>
    T* AddChild()
    {
        T* ChildNode = new T();

        // 생성하는 객체가 BTNode 상속이 아니면 생성불가
        if (nullptr == dynamic_cast<BTNode*>(ChildNode))
        {
            delete ChildNode;
            return nullptr;
        }

        ChildNode->SetRootNode((BTNode*)this);     // 루트 노드 설정
        m_Child.emplace_back(ChildNode);        // 자식 노드로 추가
        ChildNode->SetParentNode(this);         // 부모노드 설정
        ChildNode->m_pOwner = m_pOwner;         // 게임오브젝트 등록
        m_ChildCnt++;                           // 자식갯수 추가

        return ChildNode;
    }

    void ClearRunningNode() { if (m_RunningNode != nullptr) m_RunningNode = nullptr; }

    // ========= 블랙 보드 =========
    BB* GetBlackBoard() { return m_BlackBoard; }

public:
    Root_Node() : m_BlackBoard(nullptr), m_RunningNode(nullptr)  { m_BlackBoard = new BB(); }
    virtual ~Root_Node() { DELETE_UNVAILUBLE(m_BlackBoard); m_RunningNode = nullptr; }

    friend class CBehaviorTree;
};

// ==================================================================
// ========================= [ 컴포짓 노드 ] =========================
// ==================================================================
#pragma region Composite Node
// 분기의 루트와 해당 분기가 실행되는 방식의 기본 규칙
class Composite_Node : public BTNode
{
public:
    Composite_Node() {}
    virtual ~Composite_Node() {}
};

// 시퀀스 노드
class Sequence_Node : public Composite_Node
{
public:
    virtual BT_STATUS Run() override;
};

// 셀렉터 노드
class Selector_Node : public Composite_Node
{
public:
    virtual BT_STATUS Run() override;
};

// 랜덤셀렉터 노드
class RandSelector_Node : public Composite_Node
{
public:
    virtual BT_STATUS Run() override;
};

#pragma endregion

// ==================================================================
// ========================= [ 데코레이터 노드 ] =====================
// ==================================================================
#pragma region Decorator Node
class Decorator_Node : public BTNode
{
public:
    virtual BT_STATUS Run() = 0;

    Decorator_Node() {}
    virtual ~Decorator_Node() {}
    
};
#pragma endregion


// ===================================================================
// ========================= [ 테스크(리프) 노드 ] ====================
// ===================================================================
#pragma region Task Node
class Task_Node : public BTNode
{
public:
    virtual BT_STATUS Run() = 0;

public:
    Task_Node() {}
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
    Root_Node* GetRootNode() { return m_RootNode; }
    void SetOwner(CGameObject* _Owner) { m_RootNode->SetOwner(_Owner); }

public:
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}
    
    CLONE(CBehaviorTree);

public:
    CBehaviorTree();
    ~CBehaviorTree();

    friend class BTNode;
};
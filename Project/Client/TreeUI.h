#pragma once
#include "UI.h"

// =========
// TreeNode
// =========
class TreeUI;
class TreeNode
{
private:
    TreeUI*             m_Owner;        // 노드를 소유하고 있는 트리
    TreeNode*           m_ParentNode;   // 부모노드
    vector<TreeNode*>   m_vecChildNode; // 노드의 자식 노드
        
    string              m_strName;      // 노드의 출력 이름
    UINT                m_ID;           // 노드의 고유 ID

    DWORD_PTR           m_Data;         // 노드에 저장된 데이터


    bool                m_CategoryNode; // 항목 대표 노드
    bool                m_Hilight;      // 노드 하이라이트 처리


public:
    void SetCategoryNode(bool _category)
    {
        m_CategoryNode = _category;
    }

public:
    const string& GetName() { return m_strName; }
    DWORD_PTR GetData() { return m_Data; }


private:
    void render_update();

public:
    TreeNode();
    ~TreeNode();

    friend class TreeUI;
};



// ======
// TreeUI
// ======
class TreeUI :
    public UI
{
private:
    TreeNode*       m_RootNode; // 트리가 소유하고 있는 노드 중 루트 노드
    UINT            g_NextId;   // 생성되는 노드뒤에 붙여줄 고유 숫자
    bool            m_bShowRoot;

    TreeNode*       m_SelectedNode;
    TreeNode*       m_LbtDownNode;

    TreeNode*       m_DragNode;
    TreeNode*       m_DropNode;

    DWORD_PTR       m_dwPrevSelected;

    UI*             m_SelectInst;
    UI_DELEGATE_1   m_SelectFunc;

    UI*             m_DragDropInst;
    UI_DELEGATE_2   m_DragDropFunc;

    string          m_strDragDropID;


public:
    virtual int render_update() override;

public:
    void Clear();
    TreeNode* AddItem(const string& _strNodeName, DWORD_PTR _Data, TreeNode* _pParent = nullptr);
    void ShowRoot(bool _Show) { m_bShowRoot = _Show; }

    void AddDynamic_Select(UI* _UI, UI_DELEGATE_1 _MemFunc)
    {
        m_SelectInst = _UI;
        m_SelectFunc = _MemFunc;
    }

    void AddDynamic_DragDrop(UI* _UI, UI_DELEGATE_2 _MemFunc)
    {
        m_DragDropInst = _UI;
        m_DragDropFunc = _MemFunc;
    }

    void SetDragDropID(const string& _strID) { m_strDragDropID = _strID; }


private:
    void SetSelectedNode(TreeNode* _Node);   
    void SetDragNode(TreeNode* _Node);
    void SetDropNode(TreeNode* _Node);

public:
    TreeNode* GetSelectedNode() { return m_SelectedNode; }
    bool GetSelectedNode(DWORD_PTR _Data);  // 입력된 데이터와 동일한 데이터를 보유하고 있는 노드를 선택상태로 만듬


public:
    TreeUI();
    ~TreeUI();

    friend class TreeNode;
};


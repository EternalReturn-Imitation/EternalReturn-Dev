#include "pch.h"
#include "TreeUI.h"



// ========
// TreeNode
// ========
TreeNode::TreeNode()
    : m_Owner(nullptr)
    , m_ParentNode(nullptr)
    , m_ID(0)
    , m_Data(0)
    , m_CategoryNode(false)
    , m_Hilight(false)    
{
}

TreeNode::~TreeNode()
{
    Safe_Del_Vec(m_vecChildNode);
}

void TreeNode::render_update()
{
    // FinalName 만들기
    string strFinalName = m_strName;
    strFinalName += "##";

    char szBuff[100] = {};
    _itoa_s(m_ID, szBuff, 10);
    strFinalName += szBuff;

    // Flag 체크
    UINT flag = ImGuiTreeNodeFlags_DefaultOpen;

    // 자식 노드가 없으면 Lear 플래그를 설정한다(화살표 제거)
    if (m_vecChildNode.empty())    
        flag |= ImGuiTreeNodeFlags_Leaf;
    
    // 클릭 되었거나, 항목 대표 노드인 경우 Selected 플래그로 하이라이트를 준다.
    if(m_Hilight || m_CategoryNode)
        flag |= ImGuiTreeNodeFlags_Selected;

    if (ImGui::TreeNodeEx(strFinalName.c_str(), flag))
    {
        // 해당 노드에 마우스 왼클릭이 발생하면 선택노드로 지정 준다.
        if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left))
        {            
            m_Owner->m_LbtDownNode = this;
        }
        else if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
        {
            m_Owner->SetSelectedNode(this);
        }
        // 또는, 트리 재구성 이전에 선택된 노드였다면, 다시 선택노드로 처리한다.
        else if (0 != m_Owner->m_dwPrevSelected && m_Owner->m_dwPrevSelected == m_Data)
        {
            m_Owner->m_dwPrevSelected = 0;
            m_Owner->SetSelectedNode(this);
        }

        // 해당 노드 위에서 드래그 스타트 체크
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload(m_Owner->m_strDragDropID.c_str(), this, sizeof(TreeNode));
            ImGui::Text(m_strName.c_str());

            // Tree 에 드래그 노드 등록
            m_Owner->SetDragNode(this);

            ImGui::EndDragDropSource();
        }


        // 드래그 시작 후, 드랍의 후보인 경우
        if (ImGui::BeginDragDropTarget())
        {
            // 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
            const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload(m_Owner->m_strDragDropID.c_str());
            if (pPayLoad)
            {
                // DropNode 를 Tree 에 알림
                m_Owner->SetDropNode(this);
            }

            ImGui::EndDragDropTarget();
        }



        for (size_t i = 0; i < m_vecChildNode.size(); ++i)
        {
            m_vecChildNode[i]->render_update();
        }

        ImGui::TreePop();
    }

}




// ================
//      TreeUI
// ================
TreeUI::TreeUI()
    : UI("##Tree")
    , m_RootNode(nullptr)
    , g_NextId(0)
    , m_bShowRoot(true)
    , m_SelectedNode(nullptr)
    , m_dwPrevSelected(0)
    , m_SelectInst(nullptr)
    , m_SelectFunc(nullptr)    
    , m_DragDropInst(nullptr)
    , m_DragDropFunc(nullptr)
{   
    m_strDragDropID = "PayLoad";
}

TreeUI::~TreeUI()
{
    if (nullptr != m_RootNode)
        delete m_RootNode;
}

int TreeUI::render_update()
{
    if (nullptr != m_RootNode)
    {
        if (m_bShowRoot)
        {
            m_RootNode->render_update();
        }
        else
        {
            for (size_t i = 0; i < m_RootNode->m_vecChildNode.size(); ++i)
            {
                m_RootNode->m_vecChildNode[i]->render_update();
            }
        }
    }

    // Drag Drop 노드 둘다 있는 경우
    if ( (m_DragNode && m_DropNode) || (m_DragNode && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left)))
    {
        if (m_DragDropInst && m_DragDropFunc)
        {
            (m_DragDropInst->*m_DragDropFunc)((DWORD_PTR)m_DragNode, (DWORD_PTR)m_DropNode);
        }
        
        m_DragNode = nullptr;
        m_DropNode = nullptr;
    }

    return 0;
}

void TreeUI::Clear()
{
    // 이전에 선택된 노드가 있으면
    if (nullptr != m_SelectedNode)
    {
        // 이름을 기억해둔다.
        m_dwPrevSelected = m_SelectedNode->GetData();
    }

    if (nullptr != m_RootNode)
    {
        delete m_RootNode;
        m_RootNode = nullptr;
    }
}

TreeNode* TreeUI::AddItem(const string& _strNodeName, DWORD_PTR _Data, TreeNode* _pParent)
{    
    TreeNode* pNewNode = new TreeNode;

    pNewNode->m_Owner = this;
    pNewNode->m_strName = _strNodeName;
    pNewNode->m_Data = _Data;
    pNewNode->m_ID = g_NextId++;

    // 루트가 NULL 이다 ==> 트리에 들어온 최초의 데이터
    if (nullptr == m_RootNode)
    {
        // 최초 데이터 입력인데, 부모를 지정한 경우
        assert(!_pParent);

        m_RootNode = pNewNode;
    }

    // 트리에 들어온 데이터가 최초가 아니다.
    else
    {
        if (_pParent)
        {
            // 노드의 부모로 지정된 노드가 해당 트리 소속이 아니다.
            if (_pParent->m_Owner != this)
                assert(nullptr);

            // 지정된 부모의 자식으로 연결
            _pParent->m_vecChildNode.push_back(pNewNode);
            pNewNode->m_ParentNode = _pParent;
        }

        // 부모로 지정된 노드가 없는경우, 루트 밑으로 넣는다
        else
        {
            // 새로 생성한 노드를 루트노드의 자식으로 연결
            m_RootNode->m_vecChildNode.push_back(pNewNode);
            pNewNode->m_ParentNode = m_RootNode;            
        }
    }

    return pNewNode;
}


void TreeUI::SetSelectedNode(TreeNode* _Node)
{
    // 마우스를 누른 노드와 뗀 노드가 일치해야 함
    if (m_LbtDownNode != _Node)
    {
        m_LbtDownNode = nullptr;
        return;
    }        

    if (m_SelectedNode)
        m_SelectedNode->m_Hilight = false;

    m_SelectedNode = _Node;
    m_LbtDownNode = nullptr;

    if (m_SelectedNode)
    {
        m_SelectedNode->m_Hilight = true;

        if (m_SelectInst && m_SelectFunc)
        {
            (m_SelectInst->*m_SelectFunc)((DWORD_PTR)m_SelectedNode);
        }
    }
}

void TreeUI::SetDragNode(TreeNode* _Node)
{
    m_DragNode = _Node;
    m_DropNode = nullptr;
}

void TreeUI::SetDropNode(TreeNode* _Node)
{
    m_DropNode = _Node;
}

bool TreeUI::GetSelectedNode(DWORD_PTR _Data)
{
    static list<TreeNode*> queue;
    queue.clear();

    queue.push_back(m_RootNode);

    TreeNode* pCurNode = nullptr;
    while (!queue.empty())
    {
        pCurNode = queue.front();
        queue.pop_front();

        for (size_t i = 0; i < pCurNode->m_vecChildNode.size(); ++i)
        {
            queue.push_back(pCurNode->m_vecChildNode[i]);
        }

        if (pCurNode->GetData() == _Data)
        {
            SetSelectedNode(pCurNode);
            return true;
        }        
    }

    return false;
}

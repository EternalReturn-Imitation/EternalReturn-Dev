#pragma once
#include "UI.h"

class TreeUI;
class TreeNode;
class BTNode;

class BehaviorTreeUI :
    public UI
{
private:
    TreeUI*     m_Tree;
    DWORD_PTR   m_dwSelectedData;
    DWORD_PTR   m_RootNode;

public:
    virtual void tick() override;
    virtual int render_update() override;

public:
    void SetRootNode(BTNode* _Node) { m_RootNode = (DWORD_PTR)_Node; }
    void ResetNodeLinker();
    void SetTargetObject(CGameObject* _Target);
    void SetTargetToInspector(DWORD_PTR _SelectedNode);
    BTNode* GetSelectedNode();

    // 트리가 리셋된 경우, 입력으로 들어온 데이터와 동일한 노드를 선택상태로 둔다.
    void SetSelectedNodeData(DWORD_PTR _data)
    {
        m_dwSelectedData = _data;
    }

private:
    void AddNode(BTNode* _Node, TreeNode* _ParentNode);
    void DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode);

public:
    BehaviorTreeUI();
    virtual ~BehaviorTreeUI();
};


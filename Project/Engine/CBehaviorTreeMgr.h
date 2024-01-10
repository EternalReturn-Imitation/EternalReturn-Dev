#pragma once

class BTNode;
class Root_Node;

class CBehaviorTreeMgr
{
public:
    static BTNode* CreateBTNode(BTNode::NODETYPE _type, UINT _flag, BTNode* _RootNode = nullptr);

    static int SaveBehaviorTree(const wstring& _BehaviorTreePath, Root_Node* _mainRoot);
    static int SaveBTNode(BTNode* _pNode, FILE* _File);

    static Root_Node* LoadBehaviorTree(const wstring& _BehaviorTreePath);
    static BTNode* LoadBTNode(FILE* _File, BTNode* _Parent);
};


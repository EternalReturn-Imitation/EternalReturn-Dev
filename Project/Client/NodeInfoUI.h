#pragma once
#include "UI.h"

class BTNode;

class NodeInfoUI :
    public UI
{
private:
    BTNode* m_pTargetNode;


public:
    virtual void init() override;
    virtual void tick() override;
    virtual int render_update() override;

public:
    void SetTargetNode(BTNode* _Target);
    BTNode* GetTargetNode() { return m_pTargetNode; }

private:
    void ClearTargetNode();

public:
    NodeInfoUI();
    ~NodeInfoUI();
};


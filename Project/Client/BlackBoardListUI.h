#pragma once
#include "UI.h"

#include <Engine\CBehaviorTree.h>

class BlackBoardListUI :
    public UI
{
private:
    BB* m_pTargetBB;

public:
    virtual void init() override;
    virtual void tick() override;
    virtual int render_update() override;

public:
    void SetBlackBoard(BB* _Target);
    BB* GetBlackBoard() { return m_pTargetBB; }
    string GetDataStr(BB::tBBData* _Data, const char* _DataType);

public:
    BlackBoardListUI();
    ~BlackBoardListUI();
};


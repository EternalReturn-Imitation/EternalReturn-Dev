#pragma once
#include "UI.h"

class BB;

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

public:
    BlackBoardListUI();
    ~BlackBoardListUI();
};


#pragma once
#include "UI.h"

class DebugContext;

class DebugLogUI :
    public UI
{
private:
    DebugContext* dbgcontext;

public:
    virtual void init() override;
    virtual void tick() override;
    virtual int render_update() override;

public:
    DebugLogUI();
    ~DebugLogUI();
};


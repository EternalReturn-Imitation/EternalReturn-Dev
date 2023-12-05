#pragma once
#include "ComponentUI.h"

#include <Engine\CScript.h>

class ScriptUI :
    public ComponentUI
{
private:
    CScript*    m_pTargetScript;
    string      m_strScriptName;

public:
    virtual int render_update() override;

public:
    void SetScript(CScript* _Script);

public:
    ScriptUI();
    ~ScriptUI();
};


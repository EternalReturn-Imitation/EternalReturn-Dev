#pragma once
#include "UI.h"

#include <Engine\ptr.h>
#include <Engine\CRes.h>

class CGameObject;

class ComponentUI :
    public UI
{
private:
    CGameObject*            m_Target;
    const COMPONENT_TYPE    m_Type;

public:
    void SetTarget(CGameObject* _Target);
    
    CGameObject* GetTarget() { return m_Target; }

    COMPONENT_TYPE GetComponentType() { return m_Type; }

    void GetResKey(Ptr<CRes> _Res, char* _Buff, size_t _BufferSize);

public:
    virtual int render_update() override;


public:
    ComponentUI(const string& _Name, COMPONENT_TYPE _Type);
    ~ComponentUI();
};


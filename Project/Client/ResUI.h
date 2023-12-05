#pragma once
#include "UI.h"

#include <Engine\ptr.h>
#include <Engine\CRes.h>

class ResUI :
    public UI
{
private:
    Ptr<CRes>       m_TargetRes;
    const RES_TYPE  m_ResType;


public:
    virtual int render_update() override;

public:
    void SetTargetRes(Ptr<CRes> _Res) { m_TargetRes = _Res; }
    Ptr<CRes> GetTargetRes() { return m_TargetRes; }


public:
    ResUI(RES_TYPE _type);
    ~ResUI();
};


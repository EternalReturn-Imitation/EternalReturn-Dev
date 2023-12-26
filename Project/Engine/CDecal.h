#pragma once
#include "CRenderComponent.h"

class CDecal :
    public CRenderComponent
{
private:
    Ptr<CTexture>   m_DecalTex;

    bool            m_bDeferred;
    bool            m_bEmissive;


public:
    // Decal 렌더링에 광원이 적용 될 것인지 아닌지
    void SetDeferredDecal(bool _bDeferred);

    // 데칼을 광원으로 취급할것인지 아닌지
    void ActivateEmissive(bool _bActivate) { m_bEmissive = _bActivate; }

public:
    virtual void finaltick() override;
    virtual void render() override;

    CLONE(CDecal);
public:
    CDecal();
    ~CDecal();
};


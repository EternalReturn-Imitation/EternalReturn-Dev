#pragma once
#include "CRenderComponent.h"


class CMeshRender :
    public CRenderComponent
{
public:
    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;
    virtual void render_shadowmap() override;

    CLONE(CMeshRender);
public:
    CMeshRender();
    ~CMeshRender();
};


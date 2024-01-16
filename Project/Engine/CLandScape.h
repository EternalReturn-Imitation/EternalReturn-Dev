#pragma once
#include "CRenderComponent.h"

#include "ptr.h"

class CLandScape :
    public CRenderComponent
{
private:
    UINT            m_iFaceX;
    UINT            m_iFaceZ;

    Ptr<CTexture>   m_HeightMap;

public:
    void SetFace(UINT _iFaceX, UINT _iFaceZ);
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_HeightMap = _HeightMap; }

    virtual void finaltick() override;
    virtual void render() override;

private:
    void CreateMesh();

    CLONE(CLandScape);
public:
    CLandScape();
    ~CLandScape();
};


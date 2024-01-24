#pragma once
#include "UI.h"

#include <Engine/ptr.h>

class CMesh;
class CAnimEditObj;

class AnimEditUI :
    public UI
{
private:
    Ptr<CMesh> m_pSelectedMesh;  // 현재 불러온 mesh
    CAnimEditObj* m_pRenderObj;  // 클립 리스트
    tMTAnimClip m_tMTCurAnimClip;

    int m_iFrameCount;  // 설정된 초당 재생 프레임

public:
    void MeshLoad() {};

private:
    void render_cliplistwindow();
    void render_previewwindow();
    void render_infowindow();
    void render_TimeLine();
    void render_CamController();

public:
    virtual void tick() override;
    virtual void finaltick() override;
    virtual int render_update() override;

public:
    AnimEditUI();
    virtual ~AnimEditUI();
};


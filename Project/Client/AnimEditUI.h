#pragma once
#include "UI.h"

#include <Engine/ptr.h>

class CMeshData;
class CAnimEditObj;

class AnimEditUI :
    public UI
{
private:
    Ptr<CMeshData> m_pSelectedMeshData;  // 현재 불러온 메시데이터
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

    void SelectMeshData(DWORD_PTR _data);

public:
    virtual void tick() override;
    virtual void finaltick() override;
    virtual int render_update() override;

public:
    AnimEditUI();
    virtual ~AnimEditUI();
};


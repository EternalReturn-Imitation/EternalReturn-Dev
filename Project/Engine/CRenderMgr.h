#pragma once
#include "CSingleton.h"

#include "ptr.h"
#include "CTexture.h"

class CCamera;
class CLight2D;
class CStructuredBuffer;

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);
private:
    vector<CCamera*>            m_vecCam;           // 현재 레벨 내에 존재하는 카메라를 등록 받아둠
    CCamera*                    m_pEditorCam;       // 외부 에디터쪽에서 관리하는 카메라를 등록 받아둠

    vector<tDebugShapeInfo>     m_vecShapeInfo;

    vector<tLightInfo>          m_vecLight2D;
    CStructuredBuffer*          m_Light2DBuffer;

    void (CRenderMgr::* RENDER_FUNC)(void);

    Ptr<CTexture>               m_RTCopyTex;


public:
    void init();
    void render();

public:
    int RegisterCamera(CCamera* _Cam, int _idx);
    void RegisterEditorCamera(CCamera* _Cam) { m_pEditorCam = _Cam; }
    void SetRenderFunc(bool _IsPlay);
    void RegisterLight2D(const tLightInfo& _Light2D) { m_vecLight2D.push_back(_Light2D); }
    void ClearCamera() { m_vecCam.clear(); }

    void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_vecShapeInfo.push_back(_info); }
    vector<tDebugShapeInfo>& GetDebugShapeInfo() { return m_vecShapeInfo; }

    CCamera* GetMainCam() 
    { 
        if (m_vecCam.empty())
            return nullptr;

        return m_vecCam[0];
    }

    void CopyRenderTarget();

private:
    void UpdateData();
    void render_play();
    void render_editor();
    void Clear();


};


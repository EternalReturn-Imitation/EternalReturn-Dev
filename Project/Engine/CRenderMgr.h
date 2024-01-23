#pragma once
#include "CSingleton.h"

#include "ptr.h"
#include "CTexture.h"

class CCamera;
class CLight2D;
class CLight3D;
class CStructuredBuffer;
class CMRT;

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);
private:
    vector<CCamera*>            m_vecCam;           // 현재 레벨 내에 존재하는 카메라를 등록 받아둠
    CCamera* m_pEditorCam;       // 외부 에디터쪽에서 관리하는 카메라를 등록 받아둠

    vector<tDebugShapeInfo>     m_vecShapeInfo;

    vector<CLight2D*>           m_vecLight2D;
    vector<tLightInfo>          m_vecLight2DInfo;

    vector<CLight3D*>           m_vecLight3D;
    vector<tLightInfo>          m_vecLight3DInfo;

    CStructuredBuffer* m_Light2DBuffer;
    CStructuredBuffer* m_Light3DBuffer;

    CMRT* m_MRT[(UINT)MRT_TYPE::END];

    CGameObject* m_GizmoTarget;  //기즈모가 생겨야할 타겟

    void (CRenderMgr::* RENDER_FUNC)(void);

    Ptr<CTexture>               m_RTCopyTex;

public:
    void init();
    void render();

public:
    int  RegisterCamera(CCamera* _Cam, int _idx);
    void RegisterEditorCamera(CCamera* _Cam);

    void SetRenderFunc(bool _IsPlay);
    UINT RegisterLight2D(CLight2D* _Light2D, const tLightInfo& _Info)
    {
        m_vecLight2D.push_back(_Light2D);
        m_vecLight2DInfo.push_back(_Info);
        return (UINT)(m_vecLight2D.size() - 1);
    }

    UINT RegisterLight3D(CLight3D* _Light3D, const tLightInfo& _Info)
    {
        m_vecLight3D.push_back(_Light3D);
        m_vecLight3DInfo.push_back(_Info);
        return (UINT)(m_vecLight3D.size() - 1);
    }
    void ClearCamera() { m_vecCam.clear(); }

    void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_vecShapeInfo.push_back(_info); }
    vector<tDebugShapeInfo>& GetDebugShapeInfo() { return m_vecShapeInfo; }

    const vector<CLight2D*>& GetLight2D() { return m_vecLight2D; }
    const vector<CLight3D*>& GetLight3D() { return m_vecLight3D; }

    CGameObject* GetGizmoTarget() { return m_GizmoTarget; }   //기즈모가 생겨야할 타겟오브젝트 게터, 세터 함수
    void SetGizmoTarget(CGameObject* _Object) { m_GizmoTarget = _Object; }

    CCamera* GetMainCam();

    void CopyRenderTarget();

    CMRT* GetMRT(MRT_TYPE _Type) { return m_MRT[(UINT)_Type]; }

private:
    void UpdateData();
    void render_shadowmap();
    void render_play();
    void render_editor();
    void Clear();

    void CreateMRT();
    void ClearMRT();


};


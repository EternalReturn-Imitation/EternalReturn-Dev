#pragma once
#include "UI.h"

#include <Engine/ptr.h>

class CMeshData;
class CAnimEditObj;
class CAnimator3D;
class CAnim3D;
class CBone;

class AnimEditUI :
    public UI
{
private:
    CAnimEditObj* m_pRenderObj;         // 현재 재생되는 오브젝트
    CAnimator3D*  m_pCurAnimator;       // 현재 선택된 애니메이션
    bool          m_bPlay;

    tMTAnimClip   m_tTempAnimclip;      // 임시 저장용 AnimClip 정보

    int           m_iFrameCount;        // 설정된 초당 재생 프레임

private:
    void print_strElement(const char* _BtnTitle, const char* _str, Vec2 _Btnsize = Vec2(0, 0));
    void print_intElement(const char* _BtnTitle, int& _int, Vec2 _Btnsize = Vec2(0, 0));
    void print_doubleElement(const char* _BtnTitle, double& _double, Vec2 _Btnsize = Vec2(0, 0));

public:
    void MeshLoad() {};

private:
    void render_menubar();
    void render_cliplistwindow();
    void render_previewwindow();
    void render_infowindow();
    void render_TimeLine();
    void render_CamController();
    void render_window();

    void SelectMeshData(DWORD_PTR _data);
    void SelectBone(DWORD_PTR _data);

public:
    virtual void tick() override;
    virtual void finaltick() override;
    virtual int render_update() override;

public:
    AnimEditUI();
    virtual ~AnimEditUI();
};


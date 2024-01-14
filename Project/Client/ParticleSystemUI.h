#pragma once
#include "ComponentUI.h"

class ParticleSystemUI :
    public ComponentUI
{
private:
    bool m_bParticleInfoUI;
    tParticleModule m_tTempParticleData;
    UINT m_iCurTab;

private:
    void OnOffBtn(int& _ModuleCheck, const char* _Lavel, UINT _id);

public:
    virtual int render_update() override;
    void render_ParticleInfoWindow();
    void render_TabBar(ImGuiTabBarFlags _flags);

public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};


#include "pch.h"
#include "SoundUI.h"

#include <Engine\CSound.h>

SoundUI::SoundUI()
    : ResUI(RES_TYPE::SOUND)
{
    SetName("Sound");
}

SoundUI::~SoundUI()
{
}

int SoundUI::render_update()
{
    ResUI::render_update();

    Ptr<CSound> sound = (CSound*)GetTargetRes().Get();

    if (ImGui::Button("SoundPlay"))
    {
        sound->Play(1, 0.5, false);
    }

    if (ImGui::Button("SoundStop"))
    {
        sound->Stop();
    }
    

    return 0;
}
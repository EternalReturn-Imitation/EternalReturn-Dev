#include "pch.h"
#include "SoundUI.h"

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

    return 0;
}
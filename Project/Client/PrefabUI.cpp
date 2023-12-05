#include "pch.h"
#include "PrefabUI.h"

PrefabUI::PrefabUI()
    :ResUI(RES_TYPE::PREFAB)
{
    SetName("Prefab");
}

PrefabUI::~PrefabUI()
{

}

int PrefabUI::render_update()
{
    ResUI::render_update();

    return 0;
}
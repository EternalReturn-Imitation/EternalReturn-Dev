#include "pch.h"
#include "MeshDataUI.h"

MeshDataUI::MeshDataUI()
    : ResUI(RES_TYPE::MESHDATA)
{
    SetName("MeshData");
}

MeshDataUI::~MeshDataUI()
{
}


int MeshDataUI::render_update()
{
    ResUI::render_update();

    return 0;
}
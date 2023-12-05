#include "pch.h"
#include "GraphicsShaderUI.h"

GraphicsShaderUI::GraphicsShaderUI()
    : ResUI(RES_TYPE::GRAPHICS_SHADER)
{
    SetName("GraphicsShader");
}

GraphicsShaderUI::~GraphicsShaderUI()
{
}

int GraphicsShaderUI::render_update()
{
    ResUI::render_update();

    return 0;
}
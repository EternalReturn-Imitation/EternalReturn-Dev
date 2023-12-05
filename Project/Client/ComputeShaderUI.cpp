#include "pch.h"
#include "ComputeShaderUI.h"

ComputeShaderUI::ComputeShaderUI()
    : ResUI(RES_TYPE::COMPUTE_SHADER)
{
    SetName("ComputeShader");
}

ComputeShaderUI::~ComputeShaderUI()
{
}

int ComputeShaderUI::render_update()
{
    ResUI::render_update();

    return 0;
}
#include "pch.h"
#include "Animator2DUI.h"


Animator2DUI::Animator2DUI()
    : ComponentUI("##Animator2D", COMPONENT_TYPE::ANIMATOR2D)
{
    SetName("Animator2D");    
}

Animator2DUI::~Animator2DUI()
{
}


int Animator2DUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;




    return TRUE;
}
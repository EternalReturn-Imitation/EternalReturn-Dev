#include "pch.h"
#include "Collider2DUI.h"


Collider2DUI::Collider2DUI()
    : ComponentUI("##Coillider2D", COMPONENT_TYPE::COLLIDER2D)
{
    SetName("Coillider2D");
}

Collider2DUI::~Collider2DUI()
{
}

int Collider2DUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;




    return TRUE;
}
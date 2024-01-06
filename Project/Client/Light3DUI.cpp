#include "pch.h"
#include "Light3DUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CLight3D.h>

Light3DUI::Light3DUI()
    : ComponentUI("##Light3D", COMPONENT_TYPE::LIGHT3D)
    , vMemLightAmbient{}
    , fMemRadius(0.f)
    , fMemAngle(0.f)
    , bDebug(false)
{
    SetName("Light3D");
}

Light3DUI::~Light3DUI()
{
}

int Light3DUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

    LIGHT_TYPE eLightType = GetTarget()->Light3D()->GetLightType();
    Vec3 vLightColor = GetTarget()->Light3D()->GetLightColor();
    Vec3 vLightAmbient = GetTarget()->Light3D()->GetLightAmbient();
    float fRadius = GetTarget()->Light3D()->GetRadius();
    float fAngle = GetTarget()->Light3D()->GetAngle();
    bool bDebug = GetTarget()->Light3D()->IsDebug();

    const char* lighttype[] = { "DIRECTIONAL", "POINT", "SPOT" };
    int CurrentType = (UINT)eLightType;

   

    ImGui::Text("LightType   ");
    ImGui::SameLine();
    ImGui::Combo("##LightType", &CurrentType, lighttype, IM_ARRAYSIZE(lighttype));

    // switch (eLightType)
    // {
    // case LIGHT_TYPE::DIRECTIONAL:
    //     if (vMemLightAmbient != Vec3{ 0.f,0.f,0.f }
    //         && vLightAmbient == Vec3{ 0.f,0.f,0.f })
    //         vLightAmbient = vMemLightAmbient;
    // 
    //     vMemLightAmbient = vLightAmbient;
    // 
    //     fAngle = 0.f;
    //     fRadius = 0.f;
    //     break;
    // case LIGHT_TYPE::SPOT:
    //     if (fMemAngle != 0.f
    //         && fAngle == 0.f)
    //         fAngle = fMemAngle;
    // 
    //     fMemAngle = fAngle;
    // case LIGHT_TYPE::POINT:
    //     if (fMemRadius != 0.f
    //         && fRadius == 0.f)
    //         fRadius = fMemRadius;
    // 
    //     fMemRadius = fRadius;
    //     vLightAmbient = {};
    //     break;
    // }

    ImGui::Text("LightColor  ");
    ImGui::SameLine();
    ImGui::DragFloat3("##LightColor", vLightColor, 0.001f, 0.f, 1.f);

    if (CurrentType == (UINT)LIGHT_TYPE::DIRECTIONAL)
    {
        ImGui::Text("LightAmbient");
        ImGui::SameLine();
        ImGui::DragFloat3("##LightAmbient", vLightAmbient, 0.001f, 0.f, 1.f);
    }
    
    if (CurrentType == (UINT)LIGHT_TYPE::POINT)
    {
        ImGui::Text("Radius      ");
        ImGui::SameLine();
        ImGui::DragFloat("##Radius", &fRadius, 1.f, 0.f, 10000.f);
    }

    if (CurrentType == (UINT)LIGHT_TYPE::SPOT)
    {
        ImGui::Text("Angle       ");
        ImGui::SameLine();
        ImGui::DragFloat("##Angle", &fAngle, 1.f, 0.f, 359.f);
    }

    if (CurrentType == (UINT)LIGHT_TYPE::POINT
        || CurrentType == (UINT)LIGHT_TYPE::SPOT)
    {
        ImGui::Text("ShowDebug   ");
        ImGui::Checkbox("##LightDebug", &bDebug);
    }

    GetTarget()->Light3D()->SetLightType((LIGHT_TYPE)CurrentType);
    GetTarget()->Light3D()->SetLightColor(vLightColor);
    GetTarget()->Light3D()->SetLightAmbient(vLightAmbient);
    GetTarget()->Light3D()->SetRadius(fRadius);
    GetTarget()->Light3D()->SetAngle(fAngle);
    GetTarget()->Light3D()->SetDebug(bDebug);

    return TRUE;
}
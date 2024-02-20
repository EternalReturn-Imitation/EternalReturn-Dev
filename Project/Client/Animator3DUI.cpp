#include "pch.h"
#include "Animator3DUI.h"

#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>

Animator3DUI::Animator3DUI()
    : ComponentUI("##Animator2D", COMPONENT_TYPE::ANIMATOR3D)
{
    SetName("Animator2D");    
}

Animator3DUI::~Animator3DUI()
{
}

int Animator3DUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

    CAnimator3D* pAnimator = GetTarget()->Animator3D();
    map<wstring,CAnim3D*> mapAnims = pAnimator->GetAnims();
    map<wstring, CAnim3D*>::iterator iter = mapAnims.begin();

    CAnim3D* pCurAnim = pAnimator->GetCurAnim();


    string strCurAnim;
    string strIterName;
    
    static int iCurAnimIdx = 0;

    if (pCurAnim)
        strCurAnim = ToString(pCurAnim->GetName());

    if (ImGui::BeginCombo("##3DAnimationCombo", strCurAnim.c_str(), 0))
    {
        for (; iter != mapAnims.end(); ++iter)
        {
            strIterName = ToString(iter->first);
            
            const bool is_selected = (strCurAnim == strIterName);
            if (ImGui::Selectable(strIterName.c_str(), is_selected))
                pAnimator->SelectAnimation(iter->first);

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    return TRUE;
}
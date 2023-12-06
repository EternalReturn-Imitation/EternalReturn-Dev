#include "pch.h"
#include "MenuUI.h"

#include "CLevelSaveLoad.h"
#include <Engine\CEventMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CScript.h>
#include <Engine/CLevelMgr.h>

#include <Script\CScriptMgr.h>



#include "ImGuiMgr.h"
#include "OutlinerUI.h"
#include "InspectorUI.h"
#include "CLevelSaveLoad.h"



MenuUI::MenuUI()
	: UI("##Menu")
{
	SetName("Menu");
}

MenuUI::~MenuUI()
{
}

void MenuUI::finaltick()
{
    if (!IsActive())
        return;

    render_update();
}

int MenuUI::render_update()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save Level"))
            {
                // Level 저장
                CLevelSaveLoad::SaveLevel(L"Level\\TestLevel.lv", CLevelMgr::GetInst()->GetCurLevel());                
            }

            if (ImGui::MenuItem("Load Level"))
            {
                // Level 불러오기
                CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(L"Level\\TestLevel.lv");

                tEvent evn = {};
                evn.Type = EVENT_TYPE::LEVEL_CHANGE;
                evn.wParam = (DWORD_PTR)pLoadedLevel;

                CEventMgr::GetInst()->AddEvent(evn);
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("GameObject"))
        {
            // 현재 레벨에 게임오브젝트 생성
            if (ImGui::MenuItem("Create Empty Object"))
            {
                CreateEmptyObject();
            }
            ImGui::Separator();

            
            if (ImGui::BeginMenu("Add Component"))
            {
                for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
                {
                    if (ImGui::MenuItem(ToString((COMPONENT_TYPE)i)))
                    {
                        AddComponent(COMPONENT_TYPE(i));
                    }
                }
                ImGui::EndMenu();
            }

            ImGui::Separator();

            if (ImGui::BeginMenu("Add Script"))
            {
                vector<wstring> vecScripts;
                CScriptMgr::GetScriptInfo(vecScripts);
                
                for (size_t i = 0; i < vecScripts.size(); ++i)
                {
                    string strScriptName = string(vecScripts[i].begin(), vecScripts[i].end());
                    if (ImGui::MenuItem(strScriptName.c_str()))
                    {
                        AddScript(vecScripts[i]);
                    }
                }
                
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Level"))
        {
            CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
            bool PlayEnable = true;
            bool PauseEnable = true;
            bool StopEnable = true;

            if (CurLevel->GetState() == LEVEL_STATE::PLAY)
            {
                PlayEnable = false;
                PauseEnable = true;
                StopEnable = true;
            }
            else if (CurLevel->GetState() == LEVEL_STATE::PAUSE)
            {
                PlayEnable = true;
                PauseEnable = false;
                StopEnable = true;
            }
            else if (CurLevel->GetState() == LEVEL_STATE::STOP)
            {
                PlayEnable = true;
                PauseEnable = false;
                StopEnable = false;
            }



            if (ImGui::MenuItem("Play", nullptr, nullptr, PlayEnable))
            {                
                CLevelSaveLoad::SaveLevel(L"Level\\Temp.lv", CurLevel);
                CurLevel->ChangeState(LEVEL_STATE::PLAY);
            }
            else if (ImGui::MenuItem("Pause", nullptr, nullptr, PauseEnable))
            {
                CurLevel->ChangeState(LEVEL_STATE::PAUSE);
            }
            else if (ImGui::MenuItem("Stop", nullptr, nullptr, StopEnable))
            {
                CurLevel->ChangeState(LEVEL_STATE::STOP);
                CLevel* pNewLevel = CLevelSaveLoad::LoadLevel(L"Level\\Temp.lv");
             
                tEvent evn = {};
                evn.Type = EVENT_TYPE::LEVEL_CHANGE;
                evn.wParam = DWORD_PTR(pNewLevel);
                CEventMgr::GetInst()->AddEvent(evn);

                // InspectorUI
                InspectorUI* Inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
                Inspector->SetTargetObject(nullptr);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Resource"))
        {
            if (ImGui::MenuItem("Create Emty Material"))
            {
                CreateEmptyMaterial();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

	return 0;
}

void MenuUI::CreateEmptyObject()
{
    CGameObject* pNewObject = new CGameObject;
    pNewObject->AddComponent(new CTransform);
    pNewObject->SetName(L"New Object");
    SpawnGameObject(pNewObject, Vec3(0.f, 0.f, 0.f), L"Default");

    // Outliner 를 가져온다.
    OutlinerUI* outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");

    // 새로추가된 오브젝트를 데이터로 하는 노드가 추가되면, 선택상태로 두게 한다.
    outliner->SetSelectedNodeData(DWORD_PTR(pNewObject));    
}

void MenuUI::CreateEmptyMaterial()
{
    Ptr<CMaterial> pNewMtrl = new CMaterial;
    CResMgr::GetInst()->AddRes<CMaterial>(L"material\\EmptyMtrl.mtrl", pNewMtrl);
    pNewMtrl->Save(pNewMtrl->GetKey());
}

void MenuUI::AddComponent(COMPONENT_TYPE _type)
{
    // Outliner 와 Inspector 를 가져온다.
    OutlinerUI* outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");
    InspectorUI* inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");

    // 선택된 오브젝트를 가져와서 ComponentType 에 맞는 컴포넌트를 생성해서 추가한다.
    CGameObject* pSelectedObject = outliner->GetSelectedObject();

    if (nullptr == pSelectedObject)
        return;

    switch (_type)
    {
    case COMPONENT_TYPE::TRANSFORM:
        pSelectedObject->AddComponent(new CTransform);
        break;
    case COMPONENT_TYPE::COLLIDER2D:
        pSelectedObject->AddComponent(new CCollider2D);
        break;
    case COMPONENT_TYPE::COLLIDER3D:
        //pSelectedObject->AddComponent(new CCollider3D);
        break;
    case COMPONENT_TYPE::ANIMATOR2D:
        pSelectedObject->AddComponent(new CAnimator2D);
        break;
    case COMPONENT_TYPE::ANIMATOR3D:
        //pSelectedObject->AddComponent(new CAnimator3D);
        break;
    case COMPONENT_TYPE::LIGHT2D:
        pSelectedObject->AddComponent(new CLight2D);
        break;
    case COMPONENT_TYPE::LIGHT3D:
        //pSelectedObject->AddComponent(new CLight3D);
        break;
    case COMPONENT_TYPE::CAMERA:
        pSelectedObject->AddComponent(new CCamera);
        break;
    case COMPONENT_TYPE::MESHRENDER:
        pSelectedObject->AddComponent(new CMeshRender);
        break;
    case COMPONENT_TYPE::PARTICLESYSTEM:
        pSelectedObject->AddComponent(new CParticleSystem);
        break;
    case COMPONENT_TYPE::TILEMAP:
        pSelectedObject->AddComponent(new CTileMap);
        break;
    case COMPONENT_TYPE::LANDSCAPE:
        //pSelectedObject->AddComponent(new CLandScape);
        break;
    case COMPONENT_TYPE::DECAL:
        //pSelectedObject->AddComponent(new CDecal);
        break;            
    }

    // Inspector 에 새롭게 추가된 컴포넌트를 알리기 위해서 타겟을 다시 알려준다.
    inspector->SetTargetObject(pSelectedObject);

}

void MenuUI::AddScript(const wstring& _strScriptName)
{
    // Outliner 와 Inspector 를 가져온다.
    OutlinerUI* outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");
    InspectorUI* inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");

    // 선택된 오브젝트를 가져와서 ComponentType 에 맞는 컴포넌트를 생성해서 추가한다.
    CGameObject* pSelectedObject = outliner->GetSelectedObject();

    if (nullptr == pSelectedObject)
        return;

    CScript* pScript = CScriptMgr::GetScript(_strScriptName);

    pSelectedObject->AddComponent(pScript);

    inspector->SetTargetObject(pSelectedObject);
}

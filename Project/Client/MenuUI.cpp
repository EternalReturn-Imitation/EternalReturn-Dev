#include "pch.h"
#include "MenuUI.h"

#include "CLevelSaveLoad.h"
#include <Engine\CEventMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CScript.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CCollisionMgr.h>
#include <Engine\CDevice.h>

#include <Engine\CPathFindMgr.h>

#include <Script\CScriptMgr.h>

#include "ImGuiMgr.h"
#include "OutlinerUI.h"
#include "InspectorUI.h"
#include "CLevelSaveLoad.h"

#include "AnimEditUI.h"
#include "ItemDataUI.h"
#include "CharacterDataUI.h"
#include "DebugLogUI.h"
#include "ListUI.h"

#include <Commdlg.h>

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
                CLevelSaveLoad::SaveLevel(L"TestLevel.lv", CLevelMgr::GetInst()->GetCurLevel());
            }
            if (ImGui::MenuItem("Load Level"))
            {
                // Level 불러오기
                CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(L"TestLevel.lv");

                tEvent evn = {};
                evn.Type = EVENT_TYPE::LEVEL_CHANGE;
                evn.wParam = (DWORD_PTR)pLoadedLevel;

                CEventMgr::GetInst()->AddEvent(evn);

                ImGuiMgr::GetInst()->InitInspector();
            }
            ImGui::Separator();

            if (ImGui::MenuItem("GameClose"))
            {
                DestroyWindow(CEngine::GetInst()->GetMainWnd());
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

            // 현재 레벨에 게임오브젝트 생성
            if (ImGui::MenuItem("Create Object from MeshData.."))
            {
                const map<wstring, Ptr<CRes>>& mapMesh = CResMgr::GetInst()->GetResources(RES_TYPE::MESHDATA);

                ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
                pListUI->Reset("MeshData List", ImVec2(300.f, 500.f));
                for (const auto& pair : mapMesh)
                {
                    pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
                }

                pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&MenuUI::CreateObjectFromMeshData);
            }
            ImGui::Separator();


            // if (ImGui::BeginMenu("Add Component"))
            // {
            //     for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
            //     {
            //         if (ImGui::MenuItem(ToString((COMPONENT_TYPE)i)))
            //         {
            //             AddComponent(COMPONENT_TYPE(i));
            //         }
            //     }
            //     ImGui::EndMenu();
            // }

            // ImGui::Separator();

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
                CKeyMgr::GetInst()->SetCsrHide(true);

                CLevelSaveLoad::SaveLevel(L"Temp.lv", CurLevel);
                // CLevelSaveLoad::SaveLevelToDB(CurLevel);
                CurLevel->ChangeState(LEVEL_STATE::PLAY);
            }
            else if (ImGui::MenuItem("Pause", nullptr, nullptr, PauseEnable))
            {
                CKeyMgr::GetInst()->SetCsrHide(false);
                ShowCursor(true);

                CurLevel->ChangeState(LEVEL_STATE::PAUSE);
            }
            //else if (ImGui::MenuItem("Stop", nullptr, nullptr, StopEnable))
            //{
            //    CKeyMgr::GetInst()->SetCsrHide(false);
            //    ShowCursor(true);
            //
            //    CurLevel->ChangeState(LEVEL_STATE::STOP);
            //    CLevel* pNewLevel = CLevelSaveLoad::LoadLevel(L"Temp.lv");
            //    // CLevel* pNewLevel = CLevelSaveLoad::LoadLevelByDB();
            //
            //    // 충돌체 매니저 초기화
            //    CCollisionMgr::GetInst()->Clear();
            //
            //    tEvent evn = {};
            //    evn.Type = EVENT_TYPE::LEVEL_CHANGE;
            //    evn.wParam = DWORD_PTR(pNewLevel);
            //    CEventMgr::GetInst()->AddEvent(evn);
            //
            //    // InspectorUI
            //    InspectorUI* Inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
            //    Inspector->SetTargetObject(nullptr);
            //}

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Resource"))
        {
            if (ImGui::MenuItem("Create Emty Material"))
            {
                CreateEmptyMaterial();
            }

            if (ImGui::MenuItem("Load FBX.."))
            {
                LoadFBX();
            }

            if (ImGui::MenuItem("Load FBX SingleMesh.."))
            {
                LoadFBX(1);
            }

            if (ImGui::MenuItem("Load Bone from FBX.."))
            {
                LoadFBX_Bone();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Editor"))
        {
            AnimEditUI* animedit = (AnimEditUI*)ImGuiMgr::GetInst()->FindUI("##AnimEditUI");
            ItemDataUI* itemdataUI = (ItemDataUI*)ImGuiMgr::GetInst()->FindUI("##ItemDataUI");
            CharacterDataUI* characterdataUI = (CharacterDataUI*)ImGuiMgr::GetInst()->FindUI("##CharacterDataUI");

            if (ImGui::MenuItem("AnimationEditor"))
            {
                animedit->SetActive(true);
                itemdataUI->SetActive(false);
                characterdataUI->SetActive(false);
            }


            if (ImGui::MenuItem("ItemDataEditor"))
            {
                animedit->SetActive(false);
                itemdataUI->SetActive(true);
                characterdataUI->SetActive(false);
            }


            if (ImGui::MenuItem("CharacterDataEditor"))
            {
                animedit->SetActive(false);
                itemdataUI->SetActive(false);
                characterdataUI->SetActive(true);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Debug"))
        {
            if (ImGui::MenuItem("DebugWindow"))
            {
                DebugLogUI* dbglogUI = (DebugLogUI*)ImGuiMgr::GetInst()->FindUI("##DebugLogUI");

                dbglogUI->SetActive(true);
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
    CResMgr::GetInst()->AddRes<CMaterial>(L"EmptyMtrl", L"material\\", L".mtrl", pNewMtrl);
    pNewMtrl->Save(pNewMtrl->GetRelativePath());
}

void MenuUI::CreateObjectFromMeshData(DWORD_PTR _pMeshData)
{
    string strKey = (char*)_pMeshData;
    Ptr<CMeshData> temp = CResMgr::GetInst()->FindRes<CMeshData>(wstring(strKey.begin(), strKey.end()));

    if (nullptr != temp)
    {
        CGameObject* pNewObject = nullptr;
        pNewObject = temp->Instantiate();
        
        pNewObject->SetName(ToWString(strKey));
        SpawnGameObject(pNewObject, L"Default");
        // SpawnChlidGameObject(pNewObject, L"Default");
        
        // Outliner 를 가져온다.
        OutlinerUI* outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");

        // 새로추가된 오브젝트를 데이터로 하는 노드가 추가되면, 선택상태로 두게 한다.
        outliner->SetSelectedNodeData(DWORD_PTR(pNewObject));
    }
    else
    {
        wstring errMsg = L"This MeshData Select Fail.";
        MessageBox(nullptr, errMsg.c_str(), L"FAIL", MB_OK);
        return;
    }
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
        // pSelectedObject->AddComponent(new CAnimator3D);
        break;
    case COMPONENT_TYPE::LIGHT2D:
        pSelectedObject->AddComponent(new CLight2D);
        break;
    case COMPONENT_TYPE::LIGHT3D:
        pSelectedObject->AddComponent(new CLight3D);
        break;
    case COMPONENT_TYPE::CAMERA:
        pSelectedObject->AddComponent(new CCamera);
        break;
    case COMPONENT_TYPE::BEHAVIORTREE:
        pSelectedObject->AddComponent(new CBehaviorTree);
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
        // pSelectedObject->AddComponent(new CLandScape);
        break;
    case COMPONENT_TYPE::DECAL:
        pSelectedObject->AddComponent(new CDecal);
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

void MenuUI::LoadFBX(int _MeshCnt)
{
    // open a file name

    OPENFILENAME ofn = {};
    wstring strFBXFolderPath = CPathMgr::GetInst()->GetContentPath();
    strFBXFolderPath += L"fbx\\";

    wchar_t szFullFilePath[256] = {};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFullFilePath;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 256;
    ofn.lpstrFilter = L"fbx\0*.fbx";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = strFBXFolderPath.c_str();
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


    ShowCursor(true);

    if (false == GetOpenFileName(&ofn))
    {
        // ShowCursor(false);
        return;
    }
    // ShowCursor(false);
    
    wstring strFilePath = szFullFilePath;
    strFilePath = strFilePath.substr(strFilePath.find(L"fbx\\"), lstrlenW(szFullFilePath));
    
    if (nullptr == CResMgr::GetInst()->LoadFBX(strFilePath.c_str(), _MeshCnt))
    {
        wstring errMsg = L"FBX File Load Fail.";
        MessageBox(nullptr, errMsg.c_str(), L"FAIL", MB_OK);
        return;
    }
    else
    {
        wstring errMsg = L"FBX File Load Compleat.\nMeshData Created.\n";
        MessageBox(nullptr, errMsg.c_str(), L"SUCCESS", MB_OK);
        return;
    }
}

void MenuUI::LoadFBX_Bone()
{
    // open a file name

    OPENFILENAME ofn = {};
    wstring strFBXFolderPath = CPathMgr::GetInst()->GetContentPath();
    strFBXFolderPath += L"fbx\\";

    wchar_t szFullFilePath[256] = {};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFullFilePath;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 256;
    ofn.lpstrFilter = L"fbx\0*.fbx";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = strFBXFolderPath.c_str();
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


    ShowCursor(true);

    if (false == GetOpenFileName(&ofn))
    {
        // ShowCursor(false);
        return;
    }
    // ShowCursor(false);

    wstring strFilePath = szFullFilePath;
    strFilePath = strFilePath.substr(strFilePath.find(L"fbx\\"), lstrlenW(szFullFilePath));

    if (nullptr == CResMgr::GetInst()->LoadFBXBone(strFilePath.c_str()))
    {
        wstring errMsg = L"Bone Data Load from FBX Fail.";
        MessageBox(nullptr, errMsg.c_str(), L"FAIL", MB_OK);
        return;
    }
    else
    {
        wstring errMsg = L"Bone Data Load from FBX Compleat.\nBone File Created.\n";
        MessageBox(nullptr, errMsg.c_str(), L"SUCCESS", MB_OK);
        return;
    }
}

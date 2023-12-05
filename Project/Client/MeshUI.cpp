#include "pch.h"
#include "MeshUI.h"

#include <Engine\CMesh.h>

MeshUI::MeshUI()
    : ResUI(RES_TYPE::MESH)
{
    SetName("Mesh");
}

MeshUI::~MeshUI()
{
}

int MeshUI::render_update()
{
    ResUI::render_update();

    ImGui::Text("Mesh   ");
    ImGui::SameLine();

    Ptr<CMesh> pMesh = (CMesh*)GetTargetRes().Get();
    string strKey = string(pMesh->GetKey().begin(), pMesh->GetKey().end());
    ImGui::InputText("##MeshUIName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
    
    return 0;
}
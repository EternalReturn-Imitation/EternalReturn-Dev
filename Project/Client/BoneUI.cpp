#include "pch.h"
#include "BoneUI.h"

#include <Engine\CBone.h>

BoneUI::BoneUI()
	: ResUI(RES_TYPE::BONE)
{
	SetName("Bone");
}

BoneUI::~BoneUI()
{
}

int BoneUI::render_update()
{
	ResUI::render_update();

	ImGui::Text("Bone   ");
	ImGui::SameLine();

	Ptr<CBone> pBone = (CBone*)GetTargetRes().Get();
	string strKey = string(pBone->GetKey().begin(), pBone->GetKey().end());
	ImGui::InputText("##BoneUIName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	return 0;
}
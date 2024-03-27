#include "pch.h"
#include "Collider3DUI.h"

#include <Engine\CCollider3D.h>
#include <Engine\CGameObject.h>

Collider3DUI::Collider3DUI()
    : ComponentUI("##Collider3D", COMPONENT_TYPE::COLLIDER3D)
{
    SetName("Collider3D");
}

Collider3DUI::~Collider3DUI()
{
}

int Collider3DUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

	CCollider3D* collider3d = GetTarget()->Collider3D();

	// ÇÁ¸®ÆÕ Å°
	ImGui::Button("Key", ImVec2(40.f, 0.f)); ImGui::SameLine();
	ImGui::SameLine();
	// wchar_t -> UTF-8
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	char szbuffer[256] = {};
	strcpy_s(szbuffer, converter.to_bytes(collider3d->GetPrefabKey()).c_str());

	ImGui::PushItemWidth(-FLT_MIN);
	ImGui::InputText("##PrefabKey", szbuffer, sizeof(szbuffer));

	// UTF-8 ->wchar_t
	std::string utf8String = szbuffer;
	std::wstring WString = converter.from_bytes(utf8String);

	collider3d->SetPrefabKey(WString);    // ÇÁ¸®ÆÕÅ° ¼öÁ¤

	ImGui::PushItemWidth(-FLT_MIN);
	if (ImGui::Button("SAVE##collider3dPrepab"))
	{
		collider3d->SavePrefab(WString);
	}
	ImGui::SameLine();
	if (ImGui::Button("SetObjName##collider3dPrepab"))
	{
		collider3d->SetPrefabKey(GetTarget()->GetName());
		collider3d->SavePrefab(GetTarget()->GetName());
	}

	ImGui::Separator();

	Vec3 vPos = GetTarget()->Collider3D()->GetOffsetPos();
	Vec3 vScale = GetTarget()->Collider3D()->GetOffsetScale();
	
	ImGui::Text("OffsetPos");
	ImGui::SameLine();
	ImGui::DragFloat3("##Offset Position", vPos);
	
	ImGui::Text("Scale   ");
	ImGui::SameLine();
	ImGui::DragFloat3("##Offset Scale", vScale);
	
	GetTarget()->Collider3D()->SetOffsetPos(vPos);
	GetTarget()->Collider3D()->SetOffsetScale(vScale);

    return TRUE;
}
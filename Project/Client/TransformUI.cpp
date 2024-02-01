#include "pch.h"
#include "TransformUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

//기즈모 관련
#include <Engine/CCamera.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CTransform.h>

#include "ImGui/ImGuizmo.h"

#include <Engine/CKeyMgr.h>
#include <Script/CCameraMoveScript.h>

static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

TransformUI::TransformUI()
	: ComponentUI("##Transform", COMPONENT_TYPE::TRANSFORM)	
{
	SetName("Transform");
}

TransformUI::~TransformUI()
{
}

int TransformUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	/*Vec3 vPos = GetTarget()->Transform()->GetRelativePos();
	Vec3 vScale = GetTarget()->Transform()->GetRelativeScale();
	Vec3 vRotation = GetTarget()->Transform()->GetRelativeRot();
	vRotation = (vRotation / XM_PI) * 180.f;

	ImGui::Text("Position");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Position", vPos);

	ImGui::Text("Scale   ");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Scale", vScale);

	ImGui::Text("Rotation");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Rotation", vRotation);

	GetTarget()->Transform()->SetRelativePos(vPos);
	GetTarget()->Transform()->SetRelativeScale(vScale);

	vRotation = (vRotation / 180.f) * XM_PI;
	GetTarget()->Transform()->SetRelativeRot(vRotation);*/

	//ImGui 렌더전에 기즈모 렌더를 해야함 ! (타겟오브젝트가 있을경우)
	if (CRenderMgr::GetInst()->GetGizmoTarget())
	{
		CGameObject* TargetObj = CRenderMgr::GetInst()->GetGizmoTarget();
		if (TargetObj->Transform())// 트랜스폼을 가지고있다면
		{
			if (!CRenderMgr::GetInst()->GetMainCam()->GetOwner()->GetScript<CCameraMoveScript>()->GetRBTNPressed()) {
				if (KEY_TAP(KEY::Q))
					mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
				else if (KEY_TAP(KEY::W))
					mCurrentGizmoOperation = ImGuizmo::ROTATE;
				else if (KEY_TAP(KEY::E))
					mCurrentGizmoOperation = ImGuizmo::SCALE;
			}			

			if (TargetObj->Transform()->GetGizmoOnSet()) //기즈모를 배치할수 있는 오브젝트라면
				RenderGizmo();  //기즈모 렌더 처리
		}
	}

	bool b_IsWindowMode = ImGuiMgr::GetInst()->GetGizmoEditor_WindowMode();
	if (!b_IsWindowMode)
	{
		ImGui::Checkbox("Window Mode", &b_IsWindowMode);
		if (b_IsWindowMode)
			ImGuiMgr::GetInst()->SetGizmoEditor_WindowMode(true);
		else
			ImGuiMgr::GetInst()->SetGizmoEditor_WindowMode(false);
	}

	return TRUE;
}

void TransformUI::EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition)
{
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
	static bool useSnap = false;
	static float snap[3] = { 1.f, 1.f, 1.f };
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool boundSizing = false;
	static bool boundSizingSnap = false;


	if (editTransformDecomposition)
	{
		if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		if (ImGui::RadioButton("Universal", mCurrentGizmoOperation == ImGuizmo::UNIVERSAL))
			mCurrentGizmoOperation = ImGuizmo::UNIVERSAL;
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];

		ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
		//윈도우 모드일때만 뜨도록 함
		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::DragFloat3("##Tr", matrixTranslation);
		ImGui::Text("Rotation");
		ImGui::SameLine();
		ImGui::DragFloat3("##Rt", matrixRotation);
		ImGui::Text("Scale   ");
		ImGui::SameLine();
		ImGui::DragFloat3("##Sc", matrixScale);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

		if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
				mCurrentGizmoMode = ImGuizmo::WORLD;
		}

		ImGui::Checkbox("##UseSnap", &useSnap);
		ImGui::SameLine();

		switch (mCurrentGizmoOperation)
		{
		case ImGuizmo::TRANSLATE:
			ImGui::InputFloat3("Snap", &snap[0]);
			break;
		case ImGuizmo::ROTATE:
			ImGui::InputFloat("Angle Snap", &snap[0]);
			break;
		case ImGuizmo::SCALE:
			ImGui::InputFloat("Scale Snap", &snap[0]);
			break;
		}
		ImGui::Checkbox("Bound Sizing", &boundSizing);
		if (boundSizing)
		{
			ImGui::PushID(3);
			ImGui::Checkbox("##BoundSizing", &boundSizingSnap);
			ImGui::SameLine();
			ImGui::InputFloat3("Snap", boundsSnap);
			ImGui::PopID();
		}
	}

	ImGuiIO& io = ImGui::GetIO();
	float viewManipulateRight = io.DisplaySize.x;
	float viewManipulateTop = 0;
	static ImGuiWindowFlags gizmoWindowFlags = 0;

	HWND hWnd = CEngine::GetInst()->GetMainWnd();
	RECT clientRect; // 클라이언트 영역의 사각형
	GetClientRect(hWnd, &clientRect); // 클라이언트 영역의 사각형을 얻음

	int clientX = clientRect.left; // 클라이언트 영역의 좌상단 x 좌표
	int clientY = clientRect.top; // 클라이언트 영역의 좌상단 y 좌표

	// 윈도우 좌표계로 변환
	POINT clientTopLeft = { clientX, clientY };
	ClientToScreen(hWnd, &clientTopLeft);

	int windowX = clientTopLeft.x; // 윈도우의 좌상단 x 좌표
	int windowY = clientTopLeft.y; // 윈도우의 좌상단 y 좌표


	ImGuizmo::SetRect((float)windowX, (float)windowY, io.DisplaySize.x, io.DisplaySize.y);

	//기즈모 계산 수행
	ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

	//-------기즈모 결과값 세팅----------------------
	CGameObject* GizTarget = CRenderMgr::GetInst()->GetGizmoTarget();
	Matrix WorldMat = XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(matrix));
	float matrixTranslation[3], matrixRotation[3], matrixScale[3];

	//월드행렬 분해
	ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);

	GizTarget->Transform()->SetRelativeScale(matrixScale[0], matrixScale[1], matrixScale[2]);
	GizTarget->Transform()->SetRelativePos(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
	GizTarget->Transform()->SetRelativeRot(XMConvertToRadians(matrixRotation[0]), XMConvertToRadians(matrixRotation[1]), XMConvertToRadians(matrixRotation[2]));

	//월드행렬 조합
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);
}

void TransformUI::RenderGizmo()
{
	//imgui 렌더전에 기즈모가 먼저 호출되어야함
	ImGuizmo::BeginFrame();
	int lastUsing = 0;

	Vec2 Resolution = CEngine::GetInst()->GetWindowResolution();
	ImGuiIO& io = ImGui::GetIO();

	//ImGuizmo::SetRect(0, 0, io.DisplaySize.x + 35.f, io.DisplaySize.y + 80.f);

	CGameObject* GizTarget = CRenderMgr::GetInst()->GetGizmoTarget();


	if (GizTarget)
	{
		Vec3 Rot = GizTarget->Transform()->GetRelativeRot();
		Vec3 Pos = GizTarget->Transform()->GetRelativePos();
		Vec3 Scale = GizTarget->Transform()->GetRelativeScale();

		////ImGuizmo::SetRect(100.f, 100.f, 100.f);
		//ImGuizmo::Manipulate(ViewMatArray, ProjMatArray, ImGuizmo::TRANSLATE, ImGuizmo::WORLD, WorldMatArray);
		Matrix WorldMat = GizTarget->Transform()->GetWorldMat();
		Matrix ViewMat = CRenderMgr::GetInst()->GetMainCam()->GetViewMat();
		Matrix ProjMat = CRenderMgr::GetInst()->GetMainCam()->GetProjMat();

		float WorldMatArray[16];
		float ViewMatArray[16];
		float ProjMatArray[16];

		XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(WorldMatArray), WorldMat);
		XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(ViewMatArray), ViewMat);
		XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(ProjMatArray), ProjMat);

		//-----------------------------------------------------


		// create a window and insert the inspector
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Appearing);
		ImGui::SetNextWindowSize(ImVec2(320, 340), ImGuiCond_Appearing);

		if (ImGuiMgr::GetInst()->GetGizmoEditor_WindowMode())
			ImGui::Begin("Editor");


		ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);
		if (ImGuizmo::IsUsing())
		{
			ImGui::Text("Using gizmo");
		}
		else
		{
			ImGui::Text(ImGuizmo::IsOver() ? "Over gizmo" : "");
			ImGui::SameLine();
			ImGui::Text(ImGuizmo::IsOver(ImGuizmo::TRANSLATE) ? "Over translate gizmo" : "");
			ImGui::SameLine();
			ImGui::Text(ImGuizmo::IsOver(ImGuizmo::ROTATE) ? "Over rotate gizmo" : "");
			ImGui::SameLine();
			ImGui::Text(ImGuizmo::IsOver(ImGuizmo::SCALE) ? "Over scale gizmo" : "");
		}
		ImGui::Separator();


		for (int matId = 0; matId < 1; matId++)
		{
			ImGuizmo::SetID(matId);

			EditTransform(ViewMatArray, ProjMatArray, WorldMatArray, lastUsing == matId);
			if (ImGuizmo::IsUsing())
			{
				lastUsing = matId;
			}
		}

		/*bool b_ViewBound = CRenderMgr::GetInst()->GetMainCam()->GetViewGizmoBounding();

		ImGui::Checkbox("View Gizmo Click Bounding", &b_ViewBound);
		if (b_ViewBound)
			CRenderMgr::GetInst()->GetMainCam()->SetViewGizmoBounding(true);
		else
			CRenderMgr::GetInst()->GetMainCam()->SetViewGizmoBounding(false);

		float f_GizBound = GetTarget()->Transform()->GetGizmoBounding();
		ImGui::Text("Gizmo Click Bound Size : ");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.f);
		ImGui::DragFloat("##ClickBound", &f_GizBound);
		GetTarget()->Transform()->SetGizmoBounding(f_GizBound);*/

		if (ImGuiMgr::GetInst()->GetGizmoEditor_WindowMode())
		{
			bool b_IsWindowMode = ImGuiMgr::GetInst()->GetGizmoEditor_WindowMode();
			ImGui::Checkbox("Window Mode", &b_IsWindowMode);
			if (b_IsWindowMode)
				ImGuiMgr::GetInst()->SetGizmoEditor_WindowMode(true);
			else
				ImGuiMgr::GetInst()->SetGizmoEditor_WindowMode(false);

			ImGui::End();
		}

	}
}

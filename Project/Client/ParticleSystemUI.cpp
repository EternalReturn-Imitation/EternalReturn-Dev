#include "pch.h"
#include "ParticleSystemUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CTexture.h>

#include <Engine\CParticleSystem.h>

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI("##ParticleSystem", COMPONENT_TYPE::PARTICLESYSTEM)
	, m_bParticleInfoUI(false)
	, m_iCurTab(0)
{
	SetName("ParticleSystem");
}

ParticleSystemUI::~ParticleSystemUI()
{
}

int ParticleSystemUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	if (ImGui::Button("EditParticle##ParticleSystemWindow"))
	{
		m_bParticleInfoUI = true;
		
		// 임시 저장용
		m_tTempParticleData = GetTarget()->ParticleSystem()->GetParticleData();
	}

	if (m_bParticleInfoUI)
		render_ParticleInfoWindow();

	return TRUE;
}

void ParticleSystemUI::render_ParticleInfoWindow()
{
	tParticleModule mParticle = GetTarget()->ParticleSystem()->GetParticleData();
	
	if (!ImGui::Begin("Particle_Editor", &m_bParticleInfoUI, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking))
	{
		ImGui::End();
		return;
	}

		render_TabBar(ImGuiTabBarFlags_None);

		ImGui::BeginGroup();

		ImGui::Text("Module Check ");
		ImGui::SameLine();
		
		switch (m_iCurTab)
		{
		case (UINT)PARTICLE_MODULE::PARTICLE_SPAWN:
		{
			OnOffBtn(mParticle.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN], "##PARTICLE_SPAWN", (UINT)PARTICLE_MODULE::PARTICLE_SPAWN);

			// 생성 주기
			ImGui::Text("SpawnRate	 		: ");
			ImGui::SameLine();
			ImGui::SliderInt("##SpawnRate", &mParticle.SpawnRate, 0, 3000, "%d");

			// 생성시 색상
			ImGui::Text("SpawnColor			: ");
			ImGui::SameLine();
			ImGui::ColorEdit3("##SpawnColor", mParticle.vSpawnColor);
			

			// static float begin = 10, end = 90;
			// static int begin_i = 100, end_i = 1000;
			// ImGui::DragFloatRange2("range float", &begin, &end, 0.25f, 0.0f, 100.0f, "Min: %.1f %%", "Max: %.1f %%", ImGuiSliderFlags_AlwaysClamp);
			// ImGui::DragIntRange2("range int", &begin_i, &end_i, 5, 0, 1000, "Min: %d units", "Max: %d units");

			
			// 생성 크기 범위
			ImGui::Text("SpawnScale			: ");
			ImGui::SameLine();
			ImGui::DragFloatRange2("##SpawnScale", mParticle.vSpawnScaleMin, mParticle.vSpawnScaleMax, 0.25f, 1.f, 1000.f, "Min: %.1f", "Max: %.1f", ImGuiSliderFlags_AlwaysClamp);

			// 파티클 생성 박스 크기
			ImGui::Text("SpawnBoxScale	 	: ");
			ImGui::SameLine();
			ImGui::SliderFloat3("##SpawnBoxScale", mParticle.vBoxShapeScale, 1, 1000);
			
			/*
			// 원형타입 시 생성 크기
			ImGui::Text("SpawnScaleMax		: ");
			ImGui::SameLine();
			float	fSphereShapeRadius;
			*/

			// 파티클 모양 타입
			ImGui::Text("SpawnShapeType		: ");
			ImGui::SameLine();
			ImGui::Combo("##SpawnShapeType", &mParticle.SpawnShapeType, "BOX\0SPHERE\0\0");

			ImGui::Text("SpawnSpace			: ");
			ImGui::SameLine();
			ImGui::Combo("##SpawnSpaceType", &mParticle.Space, "World\0Local\0\0");
			
			ImGui::Text("LifeTime		  	: ");
			ImGui::SameLine();
			ImGui::DragFloatRange2("##SpawnLifeTime", &mParticle.MinLifeTime, &mParticle.MaxLifeTime, 0.1f, 0.1f, 20.f, "Min: %.1f", "Max: %.1f", ImGuiSliderFlags_AlwaysClamp);

			break;
		}
		case (UINT)PARTICLE_MODULE::COLOR_CHANGE:
		{
			OnOffBtn(mParticle.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE], "##COLOR_CHANGE", (UINT)PARTICLE_MODULE::COLOR_CHANGE);

			// 초기 색상
			ImGui::Text("StartColor			: ");
			ImGui::SameLine();
			ImGui::ColorEdit3("##StartColor", mParticle.vStartColor);

			// 최종시 색상
			ImGui::Text("EndColor	 	 	: ");
			ImGui::SameLine();
			ImGui::ColorEdit3("##EndColor", mParticle.vEndColor);

			break;
		}
		case (UINT)PARTICLE_MODULE::SCALE_CHANGE:
		{
			OnOffBtn(mParticle.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE], "##SCALE_CHANGE", (UINT)PARTICLE_MODULE::SCALE_CHANGE);

			// 초기 크기 배율
			ImGui::Text("StartScale			: ");
			ImGui::SameLine();
			ImGui::SliderFloat("##StartScale", &mParticle.StartScale, 0, 10, "%.1f");

			// 최종 크기 배율
			ImGui::Text("EndScale	  		: ");
			ImGui::SameLine();
			ImGui::SliderFloat("##EndScale", &mParticle.EndScale, 0, 10, "%.1f");

			break;
		}
		case (UINT)PARTICLE_MODULE::ADD_VELOCITY:
		{
			OnOffBtn(mParticle.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY], "##ADD_VELOCITY", (UINT)PARTICLE_MODULE::ADD_VELOCITY);

			ImGui::Text("Set VELOCITY Dir  	:");
			ImGui::SameLine();
			ImGui::SliderFloat3("##VelocityDir", mParticle.vVelocityDir, -1.f, 1.f, "%.2f");

			// ImGui::Text("VelocityDir[x]		: ");
			// ImGui::SameLine();
			// ImGui::SliderAngle("##VelocityDirX", &mParticle.vVelocityDir.x);
			// 
			// ImGui::Text("VelocityDir[y]		: ");
			// ImGui::SameLine();
			// ImGui::SliderAngle("##VelocityDirY", &mParticle.vVelocityDir.y);
			// 
			// ImGui::Text("VelocityDir[z]		: ");
			// ImGui::SameLine();
			// ImGui::SliderAngle("##VelocityDirZ", &mParticle.vVelocityDir.z);
			
			ImGui::Spacing();

			ImGui::Text("VelocityType	  	:");
			ImGui::SameLine();
			ImGui::Combo("##AddVelocityType", &mParticle.AddVelocityType, "From Center\0To Center\0 Fixed Direction\0\0");

			ImGui::Text("OffsetAngle  	 	: ");
			ImGui::SameLine();
			ImGui::SliderAngle("##OffsetAngle", &mParticle.OffsetAngle);
			
			ImGui::Text("Speed		 		: ");
			ImGui::SameLine();
			ImGui::SliderFloat("##VelocitySpeed", &mParticle.Speed, 0, 100.f);

			break;
		}
		case (UINT)PARTICLE_MODULE::DRAG:
		{
			OnOffBtn(mParticle.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG], "##DRAG", (UINT)PARTICLE_MODULE::DRAG);

			ImGui::Text("StartDrag	 		: ");
			ImGui::SameLine();
			ImGui::SliderFloat("##StartDrag", &mParticle.StartDrag, 0, 1000.f);

			ImGui::Text("EndDrag	  	 	: ");
			ImGui::SameLine();
			ImGui::SliderFloat("##EndDrag", &mParticle.EndDrag, 0, 1000.f);

			break;
		}
		case (UINT)PARTICLE_MODULE::NOISE_FORCE:
		{
			OnOffBtn(mParticle.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE], "##NOISE_FORCE", (UINT)PARTICLE_MODULE::NOISE_FORCE);

			ImGui::Text("NoiseTerm	 		: ");
			ImGui::SameLine();
			ImGui::SliderFloat("##NoiseTerm", &mParticle.fNoiseTerm, 0, 1000.f);

			ImGui::Text("NoiseForce			: ");
			ImGui::SameLine();
			ImGui::SliderFloat("##NoiseForce", &mParticle.fNoiseForce, 0, 1000.f);

			break;
		}
		case (UINT)PARTICLE_MODULE::RENDER:
		{
			OnOffBtn(mParticle.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER], "##RENDER", (UINT)PARTICLE_MODULE::RENDER);

			// 1 : 속도정렬 사용(이동 방향으로 회전) 0 : 사용 안함
			ImGui::Text("VelocityAlignment 	:");
			ImGui::SameLine();
			ImGui::Combo("##VelocityAlignment", &mParticle.VelocityAlignment, "OFF\0ON\0\0");
			
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Rotaition To Movedirection");

			// 1 : 속도에 따른 크기 변화 사용, 0 : 사용 안함	
			ImGui::Text("VelocityScale	 	:");
			ImGui::SameLine();
			ImGui::Combo("##VelocityScale", &mParticle.VelocityScale, "OFF\0ON\0\0");
			
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Transform scale on the velocity");


			// 최대 크기에 도달하는 속력
			ImGui::Text("MaxSpeed		  	:");
			ImGui::SameLine();
			ImGui::SliderFloat("##vMaxSpeed", &mParticle.vMaxSpeed, 0, 1000.f);

			// 속력에 따른 크기 변화량 최대치

			ImGui::Text("MaxVelocityScale  	:");
			ImGui::SameLine();
			ImGui::SliderFloat4("##vMaxVelocityScale", mParticle.vMaxVelocityScale, 0, 10.f);

			break;
		}
		default:
			break;
		}

		ImGui::EndGroup();

		GetTarget()->ParticleSystem()->SetParticleInfo(mParticle);

		if (ImGui::Button("   OK   ##ParticleEditorDoneBtn"))
		{
			m_tTempParticleData = mParticle;
			m_bParticleInfoUI = false;
		}

		ImGui::SameLine();

		if (ImGui::Button(" Cancle ##ParticleEditorCancleBtn"))
		{
			GetTarget()->ParticleSystem()->SetParticleInfo(m_tTempParticleData);
			m_bParticleInfoUI = false;
		}

		ImGui::End();
	
}

void ParticleSystemUI::render_TabBar(ImGuiTabBarFlags _flags)
{
	if (ImGui::BeginTabBar("ParticleEditorTabBar", _flags))
	{
		if (ImGui::BeginTabItem("SPAWN"))
		{
			m_iCurTab = (UINT)PARTICLE_MODULE::PARTICLE_SPAWN; ImGui::EndTabItem();
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
			ImGui::SetTooltip("Set Particle Spawn");
		
		if (ImGui::BeginTabItem("COLOR"))
		{
			m_iCurTab = (UINT)PARTICLE_MODULE::COLOR_CHANGE; ImGui::EndTabItem();
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
			ImGui::SetTooltip("Set Particle Color Change Effect");
		
		if (ImGui::BeginTabItem("SCALE"))
		{
			m_iCurTab = (UINT)PARTICLE_MODULE::SCALE_CHANGE; ImGui::EndTabItem();
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
			ImGui::SetTooltip("Set Particle Scale Change Effect");
		
		if (ImGui::BeginTabItem("VELOCITY"))
		{
			m_iCurTab = (UINT)PARTICLE_MODULE::ADD_VELOCITY; ImGui::EndTabItem();
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
			ImGui::SetTooltip("Set Particle Add Velocity Effect");
		
		if (ImGui::BeginTabItem("DRAG"))
		{
			m_iCurTab = (UINT)PARTICLE_MODULE::DRAG; ImGui::EndTabItem();
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
			ImGui::SetTooltip("Set Particle Drag Effect");
		
		if (ImGui::BeginTabItem("NOISE"))
		{
			m_iCurTab = (UINT)PARTICLE_MODULE::NOISE_FORCE; ImGui::EndTabItem();
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
			ImGui::SetTooltip("Set Particle RandomSpawn Effect");
		
		if (ImGui::BeginTabItem("RENDER"))
		{
			m_iCurTab = (UINT)PARTICLE_MODULE::RENDER; ImGui::EndTabItem();
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
			ImGui::SetTooltip("Set Particle Render Option");

		ImGui::EndTabBar();
	}
}

void ParticleSystemUI::OnOffBtn(int& _ModuleCheck, const char* _Lavel, UINT _id)
{
	bool bOn = false;
	int  BtnCol = 0;

	string strName = "OFF";

	if (0 < _ModuleCheck)
	{
		bOn = true;
		BtnCol = 2;
		strName = "ON";
	}
	strName += _Lavel;

	ImGui::PushID(_id);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(BtnCol / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(BtnCol / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(BtnCol / 7.0f, 0.8f, 0.8f));

	if (ImGui::Button(strName.c_str()))
	{
		if (bOn)
			_ModuleCheck = false;
		else
			_ModuleCheck = true;
	}

	ImGui::PopStyleColor(3);
	ImGui::PopID();
}
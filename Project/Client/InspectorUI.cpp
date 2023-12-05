#include "pch.h"
#include "InspectorUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CKeyMgr.h>

#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "Collider2DUI.h"
#include "CameraUI.h"
#include "Animator2DUI.h"
#include "TileMapUI.h"
#include "Light2DUI.h"

#include "MeshDataUI.h"
#include "TextureUI.h"
#include "MeshUI.h"
#include "SoundUI.h"
#include "PrefabUI.h"
#include "GraphicsShaderUI.h"
#include "ComputeShaderUI.h"
#include "MaterialUI.h"
#include "ScriptUI.h"



InspectorUI::InspectorUI()
	: UI("##Inspector")
	, m_pTargetObj(nullptr)
	, m_arrComUI{}	
	, m_arrResUI{}
{
	SetName("Inspector");

	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetSize(0.f, 150.f);	
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = new MeshRenderUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]);

	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = new CameraUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]);

	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new Collider2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new Animator2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP] = new TileMapUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]);

	// ResUI
	m_arrResUI[(UINT)RES_TYPE::MESHDATA] = new MeshDataUI;
	m_arrResUI[(UINT)RES_TYPE::MESHDATA]->SetSize(0.f, 0.f);
	AddChildUI(m_arrResUI[(UINT)RES_TYPE::MESHDATA]);

	m_arrResUI[(UINT)RES_TYPE::MESH] = new MeshUI;
	m_arrResUI[(UINT)RES_TYPE::MESH]->SetSize(0.f, 0.f);
	AddChildUI(m_arrResUI[(UINT)RES_TYPE::MESH]);

	m_arrResUI[(UINT)RES_TYPE::TEXTURE] = new TextureUI;
	m_arrResUI[(UINT)RES_TYPE::TEXTURE]->SetSize(0.f, 0.f);
	AddChildUI(m_arrResUI[(UINT)RES_TYPE::TEXTURE]);

	m_arrResUI[(UINT)RES_TYPE::GRAPHICS_SHADER] = new GraphicsShaderUI;
	m_arrResUI[(UINT)RES_TYPE::GRAPHICS_SHADER]->SetSize(0.f, 0.f);
	AddChildUI(m_arrResUI[(UINT)RES_TYPE::GRAPHICS_SHADER]);

	m_arrResUI[(UINT)RES_TYPE::COMPUTE_SHADER] = new ComputeShaderUI;
	m_arrResUI[(UINT)RES_TYPE::COMPUTE_SHADER]->SetSize(0.f, 0.f);
	AddChildUI(m_arrResUI[(UINT)RES_TYPE::COMPUTE_SHADER]);

	m_arrResUI[(UINT)RES_TYPE::PREFAB] = new PrefabUI;
	m_arrResUI[(UINT)RES_TYPE::PREFAB]->SetSize(0.f, 0.f);
	AddChildUI(m_arrResUI[(UINT)RES_TYPE::PREFAB]);

	m_arrResUI[(UINT)RES_TYPE::MATERIAL] = new MaterialUI;
	m_arrResUI[(UINT)RES_TYPE::MATERIAL]->SetSize(0.f, 0.f);
	AddChildUI(m_arrResUI[(UINT)RES_TYPE::MATERIAL]);

	m_arrResUI[(UINT)RES_TYPE::SOUND] = new SoundUI;
	m_arrResUI[(UINT)RES_TYPE::SOUND]->SetSize(0.f, 0.f);
	AddChildUI(m_arrResUI[(UINT)RES_TYPE::SOUND]);
}

InspectorUI::~InspectorUI()
{
	
}

void InspectorUI::init()
{
	SetTargetObject(nullptr);
}

void InspectorUI::tick()
{
	
}

int InspectorUI::render_update()
{
	
	return TRUE;
}

void InspectorUI::SetTargetObject(CGameObject* _Target)
{
	ClearTargetResource();

	// 타겟오브젝트 정보 노출
	m_pTargetObj = _Target;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComUI[i])
			continue;

		m_arrComUI[i]->SetTarget(m_pTargetObj);
	}

	// 타겟 오브젝트가 nullptr 이면
	// 스크립트UI 들을 전부 비활성화 시킨다.
	if (nullptr == m_pTargetObj)
	{
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->SetActive(false);
		}
		return ;
	}

	// 오브젝트의 스크립트 목록을 받아온다.
	const vector<CScript*> & vecScript = m_pTargetObj->GetScripts();

	// 스크립트UI 가 스크립트 수 보다 적으면 그만큼 추가해준다.
	if (m_vecScriptUI.size() < vecScript.size())
	{
		UINT iDiffer = vecScript.size() - m_vecScriptUI.size();
		for (UINT i = 0; i < iDiffer; ++i)
		{
			ScriptUI* UI = new ScriptUI;

			m_vecScriptUI.push_back(UI);
			AddChildUI(UI);
			UI->SetActive(true);			
		}
	}

	// ScriptUI 반복문 돌면서 오브젝트의 스크립트수 만큼만 활성화 시킨다.
	for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
	{
		if (vecScript.size() <= i)
		{
			m_vecScriptUI[i]->SetActive(false);
			continue;
		}

		// 스크립트를 스크립트UI 에게 알려준다.
		m_vecScriptUI[i]->SetTarget(m_pTargetObj);
		m_vecScriptUI[i]->SetScript(vecScript[i]);
		m_vecScriptUI[i]->SetActive(true);
	}
}

void InspectorUI::SetTargetResource(Ptr<CRes> _Res)
{
	ClearTargetObject();

	for (UINT i = 0; i < UINT(RES_TYPE::END); ++i)
	{
		m_arrResUI[i]->SetActive(false);
	}

	m_pTargetRes = _Res;
		
	if (nullptr == m_pTargetRes)
		return;

	RES_TYPE type = _Res->GetType();

	m_arrResUI[(UINT)type]->SetActive(true);
	m_arrResUI[(UINT)type]->SetTargetRes(_Res);	
}

void InspectorUI::ClearTargetObject()
{
	// 타겟오브젝트 정보 노출
	m_pTargetObj = nullptr;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComUI[i])
			continue;

		m_arrComUI[i]->SetTarget(nullptr);
	}
}

void InspectorUI::ClearTargetResource()
{
	m_pTargetRes = nullptr;

	for (UINT i = 0; i < UINT(RES_TYPE::END); ++i)
	{
		if (nullptr != m_arrResUI[i])
		{
			m_arrResUI[i]->SetTargetRes(nullptr);
			m_arrResUI[i]->SetActive(false);
		}		
	}
}
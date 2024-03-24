#include "pch.h"
#include "ER_UIScript_StatusBarGauge.h"

#include "ER_GameSystem.h"
#include "ER_DataScript_Character.h"

ER_UIScript_StatusBarGauge::ER_UIScript_StatusBarGauge()
	: CScript((UINT)SCRIPT_TYPE::ER_UISCRIPT_STATUSBARGAUGE)
	, m_Data(nullptr)
	, m_Stats(nullptr)
	, m_MainRatio(0.f)
	, m_ReturnRatio(0.f)
{
}

ER_UIScript_StatusBarGauge::~ER_UIScript_StatusBarGauge()
{
}

void ER_UIScript_StatusBarGauge::RegistPlayerCharacetr()
{
	m_Data = ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_DataScript_Character>();
	m_Stats = m_Data->GetStatus();
}

float ER_UIScript_StatusBarGauge::Lerp(float CurValue, float PrevValue)
{
	if (CurValue == PrevValue)
	{
		return PrevValue;
	}
	else
	{
		float t = 0.1f;
		float adjustedT = 1.0f - pow(1.0f - t, 1.f);

		// 보간된 값을 변환
		float res = PrevValue + adjustedT * (CurValue - PrevValue);

		return res;
	}
}

void ER_UIScript_StatusBarGauge::init()
{
}

void ER_UIScript_StatusBarGauge::begin()
{
	if (m_GaugeType == 0)
		m_PrevValue = m_Stats->iMaxHP;
	else if (m_GaugeType == 0)
		m_PrevValue = m_Stats->iMaxSP;
}

void ER_UIScript_StatusBarGauge::tick()
{
	//ratio 계산
	if (m_GaugeType == 0)
	{
		float MaxHP = m_Stats->iMaxHP;
		float CurHP = m_Stats->iHP;
		
		m_MainRatio = CurHP / MaxHP;
		m_PrevValue = Lerp((float)CurHP, m_PrevValue);
		
		m_ReturnRatio = m_PrevValue / MaxHP;
	}
	else if (m_GaugeType == 1)
	{
		float MaxSP = m_Stats->iMaxSP;
		float CurSP = m_Stats->iSP;
		
		m_MainRatio = CurSP / MaxSP;
		m_PrevValue = Lerp((float)CurSP, m_PrevValue);
		m_ReturnRatio = m_PrevValue / MaxSP;
	}

	MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, &m_MainRatio);
	MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_1, &m_ReturnRatio);

	if (KEY_TAP(KEY::_8))
	{
		m_Stats->iHP -= 28;
	}
	if (KEY_TAP(KEY::_9))
	{
		m_Stats->iSP -= 23;
	}
}

void ER_UIScript_StatusBarGauge::SetUI(int type)
{
	m_GaugeType = type;

	AddComponents(GetOwner(), _TRANSFORM | _MESHRENDER);
	MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUIGuage_Mtrl"), 0);
	MeshRender()->GetDynamicMaterial(0);
	
	switch (type)
	{
	case 0:
	{
		GetOwner()->SetName(L"UI_StatusBar_HPGuage");
		MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Img_Main_Gage_01.png"));
		MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"ReturnBar_UI.png"));
		MeshRender()->GetMaterial(0)->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"Img_Main_Gage_05.png"));
		break;
	}
	case 1:
	{
		GetOwner()->SetName(L"UI_StatusBar_SPGuage");
		MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Img_Main_Gage_02.png"));
		MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"ReturnBar_UI.png"));
		break;
	}
	}

	GetOwner()->LoadAllPrefabFromObjName();
}

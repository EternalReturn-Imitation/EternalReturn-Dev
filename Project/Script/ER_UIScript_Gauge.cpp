#include "pch.h"
#include "ER_UIScript_Gauge.h"

#include "ER_DataScript_Character.h"

ER_UIScript_Gauge::ER_UIScript_Gauge()
	: CScript((UINT)SCRIPT_TYPE::ER_UISCRIPT_GAUGE)
	, m_Data(nullptr)
	, m_Stats(nullptr)
	, m_MainRatio(0.f)
	, m_ReturnRatio(0.f)
{
}

ER_UIScript_Gauge::~ER_UIScript_Gauge()
{
}

void ER_UIScript_Gauge::RegistCharacetr(ER_DataScript_Character* _CharacterData)
{
	m_Data = _CharacterData;
	m_Stats = m_Data->GetStatus();
}

float ER_UIScript_Gauge::Lerp(float CurValue, float PrevValue)
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

void ER_UIScript_Gauge::init()
{
	

}

void ER_UIScript_Gauge::begin()
{
	if (m_GaugeType == 0)
		m_PrevValue = m_Stats->iMaxHP;
	else if (m_GaugeType == 0)
		m_PrevValue = m_Stats->iMaxSP;
}

void ER_UIScript_Gauge::tick()
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
}

void ER_UIScript_Gauge::SetUI(int type)
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
		GetOwner()->SetName(L"UI_TrackingStatusBar_HPGuage");
		MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Img_Main_Gage_01.png"));
		MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"ReturnBar_UI.png"));
		break;
	}
	case 1:
	{
		GetOwner()->SetName(L"UI_TrackingStatusBar_SPGuage");
		MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Img_Main_Gage_02.png"));
		MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"ReturnBar_UI.png"));
		break;
	}
	}
	
	GetOwner()->LoadAllPrefabFromObjName();
}

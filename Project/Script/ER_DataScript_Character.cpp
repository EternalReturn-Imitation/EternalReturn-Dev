#include "pch.h"
#include "ER_DataScript_Character.h"
#include "ER_DataScript_Item.h"
#include "ER_struct.h"
#include <FontEngine\FW1FontWrapper.h>
#include "ER_DataScript_Item.h"

ER_DataScript_Character::ER_DataScript_Character()
	: CScript((UINT)SCRIPT_TYPE::ER_DATASCRIPT_CHARACTER)
	, m_Stats(nullptr)
	, m_StatusEffect(nullptr)
	, m_Skill{}
	, m_bGameDead(false)
	, m_bOutofContorl(false)
	, m_Equipment{}
	, m_Inventory{}
	, m_bHPChangeTrigger(false)
	, m_fSPRegenTime(0.f)
{
	m_Stats = new tIngame_Stats;
	m_StatusEffect = new tStatus_Effect;
}

ER_DataScript_Character::ER_DataScript_Character(const ER_DataScript_Character& _origin)
	: m_Stats(nullptr)
	, m_StatusEffect(nullptr)
	, m_strKey(_origin.m_strKey)
	, m_strName(_origin.m_strName)
	, m_STDStats(_origin.m_STDStats)
	, m_PortraitTex(_origin.m_PortraitTex)
	, m_FullTax(_origin.m_FullTax)
	, m_MapTex(_origin.m_MapTex)
	, m_Skill{}
	, m_bGameDead(false)
	, m_bOutofContorl(false)
	, m_Equipment{}
	, m_Inventory{}
	, CScript((UINT)SCRIPT_TYPE::ER_DATASCRIPT_CHARACTER)
	, m_bHPChangeTrigger(false)
	, m_fSPRegenTime(0.f)
{
	m_Stats = new tIngame_Stats;
	m_StatusEffect = new tStatus_Effect;

	for (int i = 0; i < _origin.m_SkillList.size(); ++i)
	{
		tSkill_Info* tmp = new tSkill_Info;
		*tmp = *_origin.m_SkillList[i];

		m_SkillList.push_back(tmp);
	}

	m_Skill[(UINT)SKILLIDX::E_1] = m_SkillList[(UINT)SKILLIDX::E_1];
	m_Skill[(UINT)SKILLIDX::W_1] = m_SkillList[(UINT)SKILLIDX::W_1];
	m_Skill[(UINT)SKILLIDX::E_1] = m_SkillList[(UINT)SKILLIDX::E_1];
	m_Skill[(UINT)SKILLIDX::R_1] = m_SkillList[(UINT)SKILLIDX::R_1];
}

ER_DataScript_Character::~ER_DataScript_Character()
{
	if (m_Stats)
		delete m_Stats;

	if (m_StatusEffect)
		delete m_StatusEffect;

	Safe_Del_Vec(m_SkillList);
}

void ER_DataScript_Character::StatusUpdate()
{
	ER_Ingame_Stats Updatetmp = *m_Stats;
	UINT Level = Updatetmp.iLevel - 1;


	// 레벨,경험치,HP,SP 는 갱신하지 않는다.
	// 1. 기본 스테이터스
	Updatetmp.iAttackPower = m_STDStats.iAttackPower + (Level * m_STDStats.iAttackPowerPerLevel);
	Updatetmp.iDefense = m_STDStats.iDefense + (Level * m_STDStats.iDefensePerLevel);
	Updatetmp.iMaxHP = m_STDStats.iMaxHP + (Level * m_STDStats.iMaxHPPerLevel);
	Updatetmp.fHPRegen = m_STDStats.fHPRegen + (Level * m_STDStats.fHPRegenPerLevel);
	Updatetmp.iMaxSP = m_STDStats.iMaxSP + (Level * m_STDStats.iMaxSPPerLevel);
	Updatetmp.fSPRegen = m_STDStats.fSPRegen + (Level * m_STDStats.fSPRegenPerLevel);
	Updatetmp.fAttackSpeed = m_STDStats.fAttackSpeed + m_STDStats.fWpAtkSpd;
	Updatetmp.fCriticalStrikeChance = m_STDStats.fCriticalStrikeChance;
	Updatetmp.fMovementSpeed = m_STDStats.fMovementSpeed;
	Updatetmp.fVisionRange = m_STDStats.fVisionRange;
	Updatetmp.iSkillAmplification = m_STDStats.iAttackPower + (Level * m_STDStats.iAttackPowerPerLevel);
	Updatetmp.fAtkRange = m_STDStats.fWpAtkRange;
	Updatetmp.fCriticalStrikeDamage = 0;
	Updatetmp.fCooldownReduction = 0;
	Updatetmp.fOmnisyphon = 0;
	Updatetmp.iSkillAmplification = 0;

	// 2. 아이템 추가 스테이터스
	for (int i = 0; i < (UINT)ER_ITEM_SLOT::END; ++i)
	{
		// 아이템이 장착되어있지 않다
		if (m_Equipment[i] == nullptr)
			continue;

		// 아이템 정보를 얻어와 업데이트
		tItem_Stats Itemtmp = GETITEMSTATS(m_Equipment[i]);

		Updatetmp.iAttackPower += Itemtmp.iAttackPower + (Level * Itemtmp.iAttackPowerPerLevel);
		Updatetmp.iDefense += Itemtmp.iDefense;
		Updatetmp.iMaxHP += Itemtmp.iMaxHP + (Level * Itemtmp.iMaxHPPerLevel);
		Updatetmp.fHPRegen += Itemtmp.fHPRegen + (Level * Itemtmp.fHPRegenPerLevel);
		Updatetmp.iMaxSP += Itemtmp.iMaxSP + (Level * Itemtmp.iMaxSPPerLevel);
		Updatetmp.fSPRegen += Itemtmp.fSPRegen + (Level * Itemtmp.fSPRegenPerLevel);
		Updatetmp.fAttackSpeed += Itemtmp.fAttackSpeed;
		Updatetmp.fCriticalStrikeChance += Itemtmp.fCriticalStrikeChance;
		Updatetmp.fCriticalStrikeDamage += Itemtmp.fCriticalStrikeDamage;
		Updatetmp.fMovementSpeed += Itemtmp.fMovementSpeed;
		Updatetmp.fVisionRange += Itemtmp.fVisionRange;
		Updatetmp.fCooldownReduction += Itemtmp.fCooldownReduction;
		Updatetmp.fOmnisyphon += Itemtmp.fOmnisyphon;
		Updatetmp.iSkillAmplification += Itemtmp.iSkillAmplification + (Level * Itemtmp.iSkillAmplificationPerLevel);
	}

	// 최종스탯 반영
	*m_Stats = Updatetmp;
}

void ER_DataScript_Character::HPRegen(float _magnification)
{
	// 회복량 회복 후 HP값
	float HPRegen = m_Stats->iHP + (m_Stats->fHPRegen * _magnification);
	
	// HP 자연 회복, 최대 HP면 최대HP로 고정
	m_Stats->iHP = m_Stats->iMaxHP < HPRegen ?	m_Stats->iMaxHP : HPRegen;
}

void ER_DataScript_Character::SPRegen(float _magnification)
{
	// 회복량 회복 후 HP값
	float SPRegen = m_Stats->iSP + (m_Stats->fSPRegen * _magnification);

	// HP 자연 회복, 최대 HP면 최대HP로 고정
	m_Stats->iSP = m_Stats->iMaxSP < SPRegen ? m_Stats->iMaxSP : SPRegen;
}

void ER_DataScript_Character::init()
{
	// [텍스쳐 로딩]
	if (0 < m_strKey.length())
	{
		wstring PortraitTexKey = L"Char_Portrait_";		// portrait
		wstring FullTexKey = L"Char_Full_";				// Full
		wstring MapTexKey = L"Char_Map_";				// map

		PortraitTexKey += m_strKey + L".png";
		FullTexKey += m_strKey + L".png";
		MapTexKey += m_strKey + L".png";

		m_PortraitTex = CResMgr::GetInst()->FindRes<CTexture>(PortraitTexKey);
		m_FullTax = CResMgr::GetInst()->FindRes<CTexture>(FullTexKey);
		m_MapTex = CResMgr::GetInst()->FindRes<CTexture>(MapTexKey);
	}
}

void ER_DataScript_Character::begin()
{
	// 캐릭터 초기능력치를 받아와 레벨 1로 초기화
	m_Stats->Init_To_LevelOne(m_STDStats);

	int a = 0;
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}
	StatusUpdate();


	// 지울거
	// CreateStatBar();
}

void ER_DataScript_Character::tick()
{
	// 스킬 쿨타임 갱신
	float CoolDownRatio = DT + (DT * m_Stats->fCooldownReduction);
	for (int i = 0; i < (UINT)SKILLIDX::SKILLMAXSIZE; ++i)
		m_SkillList[i]->SkillStatusUpdate(CoolDownRatio);
	
	// 버프디버프 쿨타임 갱신
	m_StatusEffect->ActionTiemUpdate(DT);

	// SPRegen
	m_fSPRegenTime += DT;
	if (0.5f <= m_fSPRegenTime)
	{
		SPRegen();
		m_fSPRegenTime -= 0.5f;
	}


	// 아래 지울것
	// UpdateStatBar();
	// //HPReturnBar업데이트
	// if (m_bHPChangeTrigger) {
	// 	ChangeHPReturnBar();
	// }
	
	//if (KEY_TAP(KEY::F)) {
	//	ChangeStatBar();
	//}
	

}

CGameObject* ER_DataScript_Character::ItemAcquisition(CGameObject* _ItemObj)
{
	int i = 0;
	//비어있는 인벤 찾음
	while (i < 10) {
		if (!m_Inventory[i])
			break;
		++i;
	}

	if (i == 10)
		return nullptr;

	m_Inventory[i] = _ItemObj;

	// std::pair<CGameObject*, CGameObject*> objPair = ER_UIMgr::GetInst()->GetInventoryItem(i / 5, i % 5);
	// objPair.first->SetEnable(true);
	// objPair.second->SetEnable(true);
	// 
	// objPair.first->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, ER_UIMgr::GetInst()->GetGradeTexture(_ItemObj->GetScript<ER_DataScript_Item>()->GetGrade()));
	// objPair.second->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, _ItemObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());

	return _ItemObj;
}

bool ER_DataScript_Character::SwapItem(CGameObject* _DragmItem, CGameObject* _DropItem)
{
	return false;
}

void ER_DataScript_Character::CreateStatBar()
{
	m_aStatBar[0] = new CGameObject;
	m_aStatBar[0]->SetName(L"UI_Player_HPBar");
	
	m_aStatBar[0]->AddComponent(new CTransform);
	m_aStatBar[0]->AddComponent(new CMeshRender);
	m_aStatBar[0]->AddComponent(new CUIComponent);
	
	m_aStatBar[0]->Transform()->SetRelativeScale(Vec3(120.f, 15.f, 1.f));
	
	m_aStatBar[0]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_aStatBar[0]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	m_aStatBar[0]->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"HPBar_UI.png"));
	m_aStatBar[0]->MeshRender()->GetDynamicMaterial(0);
	
	Vec3 ownerPos = GetOwner()->Transform()->GetRelativePos();
	// Vec3 pos = ER_UIMgr::GetInst()->WorldPosToUIPos(ownerPos);
	// 
	// SpawnGameObject(m_aStatBar[0], Vec3(pos.x, pos.y+ 62.5f, -1.1f), L"UI");



	m_aStatBar[1] = new CGameObject;
	m_aStatBar[1]->SetName(L"UI_Player_HPReturnBar");

	m_aStatBar[1]->AddComponent(new CTransform);
	m_aStatBar[1]->AddComponent(new CMeshRender);
	m_aStatBar[1]->AddComponent(new CUIComponent);

	m_aStatBar[1]->Transform()->SetRelativeScale(Vec3(120.f, 15.f, 1.f));

	m_aStatBar[1]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_aStatBar[1]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	m_aStatBar[1]->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ReturnBar_UI.png"));
	m_aStatBar[1]->MeshRender()->GetDynamicMaterial(0);

	// SpawnGameObject(m_aStatBar[1], Vec3(pos.x, pos.y + 62.5f, -1.f), L"UI");



	m_aStatBar[2] = new CGameObject;
	m_aStatBar[2]->SetName(L"UI_Player_SteminarBar");

	m_aStatBar[2]->AddComponent(new CTransform);
	m_aStatBar[2]->AddComponent(new CMeshRender);
	m_aStatBar[2]->AddComponent(new CUIComponent);

	m_aStatBar[2]->Transform()->SetRelativeScale(Vec3(120.f, 5.f, 1.f));

	m_aStatBar[2]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_aStatBar[2]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	m_aStatBar[2]->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"SPBar_UI.png"));
	m_aStatBar[2]->MeshRender()->GetDynamicMaterial(0);

	// SpawnGameObject(m_aStatBar[2], Vec3(pos.x, pos.y + 42.5f, -1.1f), L"UI");

	// Text Obj
	m_aStatBar[3] = new CGameObject;
	AddComponents(m_aStatBar[3], _TRANSFORM | _MESHRENDER);
	m_aStatBar[3]->SetName(L"UI_stat00");

	// 텍스트 출력 필수요소 : _TRANSFORM | _MESHRENDER | _TEXT
	// Std2DUIMtrl 사용
	m_aStatBar[3]->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 0.f));
	m_aStatBar[3]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_aStatBar[3]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);

	// 텍스쳐없어도되지만 텍스쳐지정 안하면 마젠타색상출력돼서 쉐이더코드처리필요
	m_aStatBar[3]->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Btn_WeaponType_Lock.png"));

	int LV = m_Stats->iLevel;

	// 폰트 넥슨Lv2고딕 과, FW1_CENTER | FW1_VCENTER Flags는 기본값으로설정해놓음.
	// 예시 : 폰트 패밀리이름(파일이름아님), OffsetPos, FontSize, RGBA값, 폰트출력 Flag.
	// m_aStatBar[3]->Text()->TextInit(L"넥슨Lv2고딕", Vec2(0.f, 0.f), 20.f, FONT_RGBA(255, 255, 255, 255), FW1_CENTER | FW1_VCENTER);
	// m_aStatBar[3]->Text()->InputString(std::to_wstring(LV));

	SpawnGameObject(m_aStatBar[3], Vec3(-542.f, -300.f, 0.f), L"UI");
}

void ER_DataScript_Character::UpdateStatBar()
{
	Vec3 ownerPos = GetOwner()->Transform()->GetRelativePos();
	// Vec3 pos = ER_UIMgr::GetInst()->WorldPosToUIPos(ownerPos);
	// 
	// m_aStatBar[0]->Transform()->SetRelativePos(Vec3(pos.x + m_aStatPosOffset[0], pos.y + 175.f, -1.1f));
	// m_aStatBar[1]->Transform()->SetRelativePos(Vec3(pos.x + m_aStatPosOffset[1], pos.y + 175.f, -1.f));
	// m_aStatBar[2]->Transform()->SetRelativePos(Vec3(pos.x + m_aStatPosOffset[2], pos.y + 165.f, -1.1f));
	// m_aStatBar[3]->Transform()->SetRelativePos(Vec3(pos.x-75.f + m_aStatPosOffset[3], pos.y + 171.25f, -1.2f));
}

void ER_DataScript_Character::ChangeStatBar()
{
	ER_Ingame_Stats* stat = GetStatus();
	float maxHP = stat->iMaxHP;
	float curHP = stat->iHP;

	//흰 바 먼저 초기화
	m_aStatBar[1]->Transform()->SetRelativePos(m_aStatBar[0]->Transform()->GetRelativePos());
	m_aStatBar[1]->Transform()->SetRelativeScale(m_aStatBar[0]->Transform()->GetRelativeScale());

	m_aStatPosOffset[1] = m_aStatPosOffset[0];

	//실제 체력바 계산
	float decreaseRate = ((curHP) / maxHP) * 120.f;
	m_aStatPosOffset[0] = -((120 - decreaseRate) / 2);

	Vec3 pos = m_aStatBar[0]->Transform()->GetRelativePos();
	pos.x = pos.x - ((120 - decreaseRate) / 2);
	Vec3 scale = m_aStatBar[0]->Transform()->GetRelativeScale();
	scale.x = decreaseRate;

	m_aStatBar[0]->Transform()->SetRelativePos(pos);
	m_aStatBar[0]->Transform()->SetRelativeScale(scale);

	m_bHPChangeTrigger = true;

	stat = GetStatus();
	float maxSR = stat->iMaxSP;
	float curSR = stat->iSP;
	
	decreaseRate = ((curSR) / maxSR) * 120.f;
	m_aStatPosOffset[2] = -((120 - decreaseRate) / 2);
	
	pos = m_aStatBar[2]->Transform()->GetRelativePos();	 
	pos.x = pos.x - ((120 - decreaseRate) / 2);
	scale = m_aStatBar[2]->Transform()->GetRelativeScale();
	scale.x = decreaseRate;
	
	m_aStatBar[2]->Transform()->SetRelativePos(pos);
	m_aStatBar[2]->Transform()->SetRelativeScale(scale);
}

void ER_DataScript_Character::ChangeHPReturnBar()
{
	Vec3 pos = m_aStatBar[1]->Transform()->GetRelativePos();
	Vec3 scale = m_aStatBar[1]->Transform()->GetRelativeScale();

	if (scale.x > m_aStatBar[0]->Transform()->GetRelativeScale().x) {
		float decreaseRate = (75.f * DT);
		scale.x -= decreaseRate;
		pos.x -= (decreaseRate / 2);
		m_aStatPosOffset[1] -=(decreaseRate / 2);
		m_aStatBar[1]->Transform()->SetRelativePos(pos);
		m_aStatBar[1]->Transform()->SetRelativeScale(scale);
	}
	else {
		m_bHPChangeTrigger = false;
	}
}

void ER_DataScript_Character::BeginOverlap(CCollider3D* _Other)
{
	
}

void ER_DataScript_Character::OnOverlap(CCollider3D* _Other)
{
}

void ER_DataScript_Character::EndOverlap(CCollider3D* _Other)
{
}

void ER_DataScript_Character::BeginRayOverlap()
{
	int a = 3;
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}
}

void ER_DataScript_Character::OnRayOverlap()
{
}

void ER_DataScript_Character::EndRayOverlap()
{
	int a = 0;
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}
}

void ER_DataScript_Character::SaveToLevelFile(FILE* _File)
{
	SaveWString(m_strKey, _File);
	SaveWString(m_strName, _File);
	fwrite(&m_STDStats,sizeof(ER_Initial_Stats),1,_File);
	SaveResRef(m_PortraitTex.Get(), _File);
	SaveResRef(m_FullTax.Get(), _File);
	SaveResRef(m_MapTex.Get(), _File);

	int SkillSize = (int)m_SkillList.size();
	fwrite(&SkillSize, sizeof(int), 1, _File);

	for (int i = 0; i < SkillSize; ++i)
	{
		m_SkillList[i]->Save(_File);
		SaveResRef(m_SkillList[i]->TexSkill.Get(), _File);
	}
}

void ER_DataScript_Character::LoadFromLevelFile(FILE* _File)
{
	LoadWString(m_strKey, _File);
	LoadWString(m_strName, _File);
	fread(&m_STDStats, sizeof(ER_Initial_Stats), 1, _File);
	LoadResRef(m_PortraitTex, _File);
	LoadResRef(m_FullTax, _File);
	LoadResRef(m_MapTex, _File);

	int SkillSize = (int)m_SkillList.size();
	fread(&SkillSize, sizeof(int), 1, _File);

	for (int i = 0; i < SkillSize; ++i)
	{
		ER_SKILL* Skill = new ER_SKILL;
		Skill->Load(_File);
		LoadResRef(Skill->TexSkill, _File);

		m_SkillList.push_back(Skill);
	}

	m_Skill[(UINT)SKILLIDX::E_1] = m_SkillList[(UINT)SKILLIDX::E_1];
	m_Skill[(UINT)SKILLIDX::W_1] = m_SkillList[(UINT)SKILLIDX::W_1];
	m_Skill[(UINT)SKILLIDX::E_1] = m_SkillList[(UINT)SKILLIDX::E_1];
	m_Skill[(UINT)SKILLIDX::R_1] = m_SkillList[(UINT)SKILLIDX::R_1];
}
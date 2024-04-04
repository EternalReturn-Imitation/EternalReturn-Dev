#pragma once
#include "pch.h"
#include "ER_AIScript_Yuki.h"

#include "ER_AIScript_Nodes.cpp"

ER_AIScript_Yuki::ER_AIScript_Yuki()
	: ER_AIScript(SCRIPT_TYPE::ER_AISCRIPT_YUKI)
{
}

ER_AIScript_Yuki::~ER_AIScript_Yuki()
{
}

void ER_AIScript_Yuki::init()
{
	RegistAllItemBox((UINT)LUMIAISLAND::HOTEL);
	Selector_Node* Sel_BattleCommand = SetERCharacterAIBase();
	SetBattleStartTime(0.f);

	//블랙보드 가져와서 0으로 배틀, 파밍 모드 초기화
	// 이후 해당 노드 만듬.
	BB* blackBoard = BehaviorTree()->GetRootNode()->GetBlackBoard();
	int a = 0;
	blackBoard->AddBBData(L"Farming_Mode", a);
	blackBoard->AddBBData(L"Battle_Mode", a);

	// BehaviorTree 구성
	// 스킬 선택
	RandSelector_Node* Sel_SelectSkill = Sel_BattleCommand->AddChild<RandSelector_Node>();

	// 스킬 성장
	Sequence_Node* Seq_SkillUp = Sel_BattleCommand->AddChild<Sequence_Node>();
	Seq_SkillUp->AddChild<Con_IsHaveSkillPoint>();	// 스킬 포인트 검사
	Seq_SkillUp->AddChild<Act_SkillLevelUp>();		// 스킬 포인트 투자

	// 스킬 구성 : 사용가능판단 -> 사용조건판단 -> 사용 -> 시전대기(Running)
	Sequence_Node* Seq_Skill_Q = Sel_SelectSkill->AddChild<Sequence_Node>();
	Seq_Skill_Q->AddChild<Con_IsActive_Skill_Q>();
	Seq_Skill_Q->AddChild<Con_IsInSkillRange>();
	Seq_Skill_Q->AddChild<Act_Skill_Q>();


	Sequence_Node* Seq_Skill_W = Sel_SelectSkill->AddChild<Sequence_Node>();
	Seq_Skill_W->AddChild<Con_IsActive_Skill_W>();
	Seq_Skill_W->AddChild<Act_Skill_W>();


	Sequence_Node* Seq_Skill_E = Sel_SelectSkill->AddChild<Sequence_Node>();
	Seq_Skill_E->AddChild<Con_IsActive_Skill_E>();
	Seq_Skill_E->AddChild<Act_Skill_E>();

	Sequence_Node* Seq_Skill_R = Sel_SelectSkill->AddChild<Sequence_Node>();
	Seq_Skill_R->AddChild<Con_IsActive_Skill_R>();
	Seq_Skill_R->AddChild<Con_IsInSkillRange>();
	Seq_Skill_R->AddChild<Act_Skill_R>();

	// 기본 공격 + 이동
	Selector_Node* Sel_AttackOrMove = Sel_BattleCommand->AddChild<Selector_Node>();

	// 공격모션
	Sequence_Node* Seq_Attack = Sel_AttackOrMove->AddChild<Sequence_Node>();
	Seq_Attack->AddChild<Act_Attack>();
}

void ER_AIScript_Yuki::begin()
{
}

void ER_AIScript_Yuki::tick()
{	if (KEY_TAP(KEY::F7)) {
		//파밍 버튼
		BB* blackBoard = BehaviorTree()->GetRootNode()->GetBlackBoard();
		blackBoard->SetBBData(L"Farming_Mode", 1);
		blackBoard->SetBBData(L"Battle_Mode", 0);
	}
	else if (KEY_TAP(KEY::F8)) {
		//전투 버튼
		BB* blackBoard = BehaviorTree()->GetRootNode()->GetBlackBoard();
		blackBoard->SetBBData(L"Farming_Mode", 0);
		blackBoard->SetBBData(L"Battle_Mode", 1);
	}

}

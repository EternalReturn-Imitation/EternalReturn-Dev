#include "pch.h"
#include "ER_ActionScript_Character.h"
#include "ER_DataScript_Character.h"

ER_ActionScript_Character::ER_ActionScript_Character(SCRIPT_TYPE _type)
	: CScript((UINT)_type)
	, FSMContext(nullptr)
	, StateList{}
	, m_Data(nullptr)
	, m_iCurState(0)
	, m_bAbleChange(bAbleChange::COMMON)
{
}

ER_ActionScript_Character::~ER_ActionScript_Character()
{
	if (FSMContext)
		delete FSMContext;

	for (int i = 0; i < ER_CHAR_ACT::END; ++i)
	{
		if (StateList[i])
			delete StateList[i];
	}
}

void ER_ActionScript_Character::begin()
{
	StateInit();
	FSMContext = new FSM(StateList[ER_CHAR_ACT::WAIT]);
	m_Data = GetOwner()->GetScript<ER_DataScript_Character>();
	
}

void ER_ActionScript_Character::tick()
{
	FSMContext->UpdateState();
}

void ER_ActionScript_Character::Wait()
{
	if (IsAbleChange(bAbleChange::COMMON))
	{
		CFindPath* findpathcomp = GetOwner()->FindPath();
	}
}

void ER_ActionScript_Character::Move(CGameObject* _Target, Vec3 _DestPos)
{
	if (IsAbleChange(bAbleChange::COMMON))
	{

		if (m_iCurState != ER_CHAR_ACT::MOVE)
			ChangeState(ER_CHAR_ACT::MOVE);
		else if (m_iCurState == ER_CHAR_ACT::MOVE)
		{
			// 플레이어블 캐릭터 특성으로 지속명령이기때문에 이곳에서 목적지 경로 설정 진행
			CFindPath* findpathcomp = GetOwner()->FindPath();
			findpathcomp->FindPath(_DestPos);
		}
	}
}

void ER_ActionScript_Character::StateInit()
{
	StateList[ER_CHAR_ACT::WAIT] = CreateWait();
	StateList[ER_CHAR_ACT::MOVE] = CreateMove();
	StateList[ER_CHAR_ACT::CRAFT] = CreateCraft();
	StateList[ER_CHAR_ACT::REST] = CreateRest();
	StateList[ER_CHAR_ACT::ATTACK] = CreateAttack();
	StateList[ER_CHAR_ACT::ARRIVE] = CreateArrive();
	StateList[ER_CHAR_ACT::DEATH] = CreateDeath();
	StateList[ER_CHAR_ACT::SKILL_Q] = CreateSkill_Q();
	StateList[ER_CHAR_ACT::SKILL_W] = CreateSkill_W();
	StateList[ER_CHAR_ACT::SKILL_E] = CreateSkill_E();
	StateList[ER_CHAR_ACT::SKILL_R] = CreateSkill_R();


	if (StateList[ER_CHAR_ACT::WAIT])
		StateList[ER_CHAR_ACT::WAIT]->SetName(L"WAIT");
	if (StateList[ER_CHAR_ACT::MOVE])
		StateList[ER_CHAR_ACT::MOVE]->SetName(L"MOVE");
	if (StateList[ER_CHAR_ACT::CRAFT])
		StateList[ER_CHAR_ACT::CRAFT]->SetName(L"CRAFT");
	if (StateList[ER_CHAR_ACT::REST])
		StateList[ER_CHAR_ACT::REST]->SetName(L"REST");
	if (StateList[ER_CHAR_ACT::ATTACK])
		StateList[ER_CHAR_ACT::ATTACK]->SetName(L"ATTACK");
	if (StateList[ER_CHAR_ACT::ARRIVE])
		StateList[ER_CHAR_ACT::ARRIVE]->SetName(L"ARRIVE");
	if (StateList[ER_CHAR_ACT::DEATH])
		StateList[ER_CHAR_ACT::DEATH]->SetName(L"DEATH");
	if (StateList[ER_CHAR_ACT::SKILL_Q])
		StateList[ER_CHAR_ACT::SKILL_Q]->SetName(L"SKILL_Q");
	if (StateList[ER_CHAR_ACT::SKILL_W])
		StateList[ER_CHAR_ACT::SKILL_W]->SetName(L"SKILL_W");
	if (StateList[ER_CHAR_ACT::SKILL_E])
		StateList[ER_CHAR_ACT::SKILL_E]->SetName(L"SKILL_E");
	if (StateList[ER_CHAR_ACT::SKILL_R])
		StateList[ER_CHAR_ACT::SKILL_R]->SetName(L"SKILL_R");
}

void ER_ActionScript_Character::ChangeState(ER_CHAR_ACT _state, bAbleChange _Grade)
{
	// 변경 가능 수준 검사
	if (IsAbleChange(_Grade))
	{
		FSMContext->ChangeState(StateList[_state]);
		m_iCurState = _state;
	}
}

bool ER_ActionScript_Character::IsAbleChange(bAbleChange _Grade)
{
	// 인자로 들어온 동작변경가능 수준이
	// 현재설정된 동작변경가능 수준과 같거나 높아야 true 반환.
	
	return m_bAbleChange <= _Grade;
}

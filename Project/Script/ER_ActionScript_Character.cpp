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
		// FindPath Component에 목적지 위치 전달
		CFindPath* findpathcomp = GetOwner()->FindPath();
		bool bMove = findpathcomp->FindPath(_DestPos);

		if (bMove && m_iCurState != ER_CHAR_ACT::MOVE)
			ChangeState(ER_CHAR_ACT::MOVE);
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
}

void ER_ActionScript_Character::ChangeState(ER_CHAR_ACT _state)
{
	FSMContext->ChangeState(StateList[_state]);
	m_iCurState = _state;
}

bool ER_ActionScript_Character::IsAbleChange(bAbleChange _able)
{
	// 인자로 들어온 동작변경가능 수준이
	// 현재설정된 동작변경가능 수준과 같거나 높아야 true 반환.
	
	return m_bAbleChange <= _able;
}

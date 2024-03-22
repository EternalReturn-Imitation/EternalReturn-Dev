#include "pch.h"
#include "ER_ActionScript_Character.h"
#include "ER_DataScript_Character.h"



ER_ActionScript_Character::ER_ActionScript_Character(SCRIPT_TYPE _type)
	: CScript((UINT)_type)
	, FSMContext(nullptr)
	, StateList{}
	, m_Data(nullptr)
	, m_iCurState(0)
	, m_iPrevState(0)
	, m_bAbleChange(bAbleChange::COMMON)
{
}

ER_ActionScript_Character::~ER_ActionScript_Character()
{
	if (FSMContext)
		delete FSMContext;

	for (int i = 0; i < (UINT)ER_CHAR_ACT::END; ++i)
	{
		if (StateList[i])
			delete StateList[i];
	}
}

void ER_ActionScript_Character::begin()
{
	StateInit();
	FSMContext = new FSM(StateList[(UINT)ER_CHAR_ACT::WAIT]);
	m_Data = GetOwner()->GetScript<ER_DataScript_Character>();

	ChangeState(ER_CHAR_ACT::ARRIVE);
}

void ER_ActionScript_Character::tick()
{
	FSMContext->UpdateState();
}

void ER_ActionScript_Character::Wait(tFSMData& _Data)
{
	if (IsAbleChange(bAbleChange::COMMON))
	{
		CFindPath* findpathcomp = GetOwner()->FindPath();
	}
}

void ER_ActionScript_Character::Move(tFSMData& _Data)
{
	if (IsAbleChange(bAbleChange::COMMON))
	{
		if (m_iCurState != (UINT)ER_CHAR_ACT::MOVE)
			ChangeState(ER_CHAR_ACT::MOVE);
		
		else if (m_iCurState == (UINT)ER_CHAR_ACT::MOVE)
		{
			// 플레이어블 캐릭터 특성으로 지속명령이기때문에 이곳에서 목적지 경로 설정 진행
			CFindPath* findpathcomp = GetOwner()->FindPath();
			Vec3 destPos = _Data.v4Data;
			findpathcomp->FindPath(destPos);
		}
	}
}

void ER_ActionScript_Character::Dead(tFSMData& _Data)
{
	m_Data->SetGameDead();
	ChangeState(ER_CHAR_ACT::DEAD,bAbleChange::DISABLE);
	SetAbleToCancle(bAbleChange::DISABLE);
}

void ER_ActionScript_Character::StateInit()
{
	StateList[(UINT)ER_CHAR_ACT::WAIT] = CreateWait();
	StateList[(UINT)ER_CHAR_ACT::MOVE] = CreateMove();
	StateList[(UINT)ER_CHAR_ACT::CRAFT] = CreateCraft();
	StateList[(UINT)ER_CHAR_ACT::REST] = CreateRest();
	StateList[(UINT)ER_CHAR_ACT::ATTACK] = CreateAttack();
	StateList[(UINT)ER_CHAR_ACT::ARRIVE] = CreateArrive();
	StateList[(UINT)ER_CHAR_ACT::DEAD] = CreateDead();
	StateList[(UINT)ER_CHAR_ACT::SKILL_Q] = CreateSkill_Q();
	StateList[(UINT)ER_CHAR_ACT::SKILL_W] = CreateSkill_W();
	StateList[(UINT)ER_CHAR_ACT::SKILL_E] = CreateSkill_E();
	StateList[(UINT)ER_CHAR_ACT::SKILL_R] = CreateSkill_R();


	if (StateList[(UINT)ER_CHAR_ACT::WAIT])
		StateList[(UINT)ER_CHAR_ACT::WAIT]->SetName(L"WAIT");
	if (StateList[(UINT)ER_CHAR_ACT::MOVE])
		StateList[(UINT)ER_CHAR_ACT::MOVE]->SetName(L"MOVE");
	if (StateList[(UINT)ER_CHAR_ACT::CRAFT])
		StateList[(UINT)ER_CHAR_ACT::CRAFT]->SetName(L"CRAFT");
	if (StateList[(UINT)ER_CHAR_ACT::REST])
		StateList[(UINT)ER_CHAR_ACT::REST]->SetName(L"REST");
	if (StateList[(UINT)ER_CHAR_ACT::ATTACK])
		StateList[(UINT)ER_CHAR_ACT::ATTACK]->SetName(L"ATTACK");
	if (StateList[(UINT)ER_CHAR_ACT::ARRIVE])
		StateList[(UINT)ER_CHAR_ACT::ARRIVE]->SetName(L"ARRIVE");
	if (StateList[(UINT)ER_CHAR_ACT::DEAD])
		StateList[(UINT)ER_CHAR_ACT::DEAD]->SetName(L"DEAD");
	if (StateList[(UINT)ER_CHAR_ACT::SKILL_Q])
		StateList[(UINT)ER_CHAR_ACT::SKILL_Q]->SetName(L"SKILL_Q");
	if (StateList[(UINT)ER_CHAR_ACT::SKILL_W])
		StateList[(UINT)ER_CHAR_ACT::SKILL_W]->SetName(L"SKILL_W");
	if (StateList[(UINT)ER_CHAR_ACT::SKILL_E])
		StateList[(UINT)ER_CHAR_ACT::SKILL_E]->SetName(L"SKILL_E");
	if (StateList[(UINT)ER_CHAR_ACT::SKILL_R])
		StateList[(UINT)ER_CHAR_ACT::SKILL_R]->SetName(L"SKILL_R");
}

bool ER_ActionScript_Character::ChangeState(ER_CHAR_ACT _state, bAbleChange _Grade)
{
	if (m_iCurState == (UINT)_state)
		return false;

	// 변경 가능 수준 검사
	if (IsAbleChange(_Grade))
	{
		m_iPrevState = m_iCurState;
		FSMContext->ChangeState(StateList[(UINT)_state]);
		m_iCurState = (UINT)_state;
	}

	return true;
	// TO_DO : 당장 변환이 불가능한경우 담아놓기.
}

bool ER_ActionScript_Character::IsAbleChange(bAbleChange _Grade)
{
	// 인자로 들어온 동작변경가능 수준이
	// 현재설정된 동작변경가능 수준과 같거나 높아야 true 반환.
	
	return m_bAbleChange <= _Grade;
}

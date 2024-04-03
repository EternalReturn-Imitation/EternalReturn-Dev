#pragma once
#include "pch.h"

#include <Engine\FSM.h>
#include <Engine\CBehaviorTree.h>
#include "ER_GameSystem.h"

#include "ER_ActionScript_Character.h"
#include "ER_ActionScript_Hyunwoo.h"
#include "ER_DataScript_Character.h"

// =======================
// ======[ 공통 노드 ]=====
// =======================

// 캐릭터 생존 판단 노드
class Con_IsArrive : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		
		if (!(CharacterData->IsDeadState()))
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
};

// 지정 시간 기준 이전인지 판단하는 노드
class Con_IsBeforeTime : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		float* GameTime = ER_GameSystem::GetInst()->GetGameTime();
		float fBattleTime = 0.f;
		GetBlackBoard()->FindBBData(L"BattleTime", fBattleTime);

		if (*GameTime <= fBattleTime)
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
};
















// 공격상태인지 판단
class Con_IsAttack : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		CGameObject* Player = ER_GameSystem::GetInst()->GetPlayerCharacter();

		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();

		if (ER_ActionScript_Character::ER_CHAR_ACT::ATTACK == Action->GetCurState())
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
};

// 공격중이 아닌지 판단
class Con_IsNotAttack : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		CGameObject* Player = ER_GameSystem::GetInst()->GetPlayerCharacter();

		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();

		if (ER_ActionScript_Character::ER_CHAR_ACT::ATTACK != Action->GetCurState())
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
};



class Act_AttackPlayer : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		CGameObject* Player = ER_GameSystem::GetInst()->GetPlayerCharacter();

		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();
		tFSMData Data = {};
		
		Data.v2Data = Player->Transform()->GetRelativePos();
		Data.lParam = (DWORD_PTR)Player;

		// 이동명령
		Action->Attack(Data);

		// 이동명령이 성공했는지 상태검사
		if (ER_ActionScript_Character::ER_CHAR_ACT::ATTACK == Action->GetCurState())
		{
			((Root_Node*)GetRootNode())->SetRunningNode(nullptr);
			return BT_SUCCESS;
		}
		else
		{
			BTNode* node = this;
			((Root_Node*)GetRootNode())->SetRunningNode(node);
			
			return BT_RUNNING;
		}
	}
};

class Act_Wait : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		CGameObject* Player = ER_GameSystem::GetInst()->GetPlayerCharacter();

		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();
		tFSMData Data = {};

		Data.v2Data = Player->Transform()->GetRelativePos();
		Data.lParam = (DWORD_PTR)Player;

		// 이동명령
		Action->Wait(Data);

		// 이동명령이 성공했는지 상태검사
		if (ER_ActionScript_Character::ER_CHAR_ACT::WAIT == Action->GetCurState())
		{
			((Root_Node*)GetRootNode())->SetRunningNode(nullptr);
			return BT_SUCCESS;
		}
		else
		{
			BTNode* node = this;
			((Root_Node*)GetRootNode())->SetRunningNode(node);

			return BT_RUNNING;
		}
	}
};
#pragma once
#include "pch.h"

#include <Engine\FSM.h>
#include <Engine\CBehaviorTree.h>
#include "ER_GameSystem.h"

#include "ER_ActionScript_Character.h"
#include "ER_ActionScript_Hyunwoo.h"
#include "ER_DataScript_Character.h"
#include "ER_DataScript_ItemBox.h"
#include "ER_DataScript_Item.h"
#include "ER_AIScript.h"

// =======================
// ======[ 공통 노드 ]=====
// =======================

// ========================================
// ======[ Condition Node : 판단 노드 ]=====
// ========================================

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

// 지정 시간 기준 이후인지 판단하는 노드
class Con_IsAfterTime : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		float* GameTime = ER_GameSystem::GetInst()->GetGameTime();
		float fBattleTime = 0.f;
		GetBlackBoard()->FindBBData(L"BattleTime", fBattleTime);

		if (fBattleTime < *GameTime)
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
};

class Con_IsAfter10Sec : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		float* GameTime = ER_GameSystem::GetInst()->GetGameTime();
		float fPrevTime = 0.f;
		GetBlackBoard()->FindBBData(L"CheckTime", fPrevTime);
		float fCheck = *GameTime - fPrevTime;
		
		GetBlackBoard()->SetBBData(L"CheckTime", *GameTime);

		// 이전 확인할때보다 5초 지났는지
		if (10.f <= fCheck)
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
};

// 스킬 포인트가 있는지
class Con_IsHaveSkillPoint : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		
		int SkillPoint = *CharacterData->GetSkillPoint();

		if (0 < SkillPoint)
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
};

// 제작가능 아이템 확인 노드 : 제작가능아이템이 있다면 첫번째 아이템 ID를 블랙보드에서 기억한다.
class Con_IsCanCraftItem : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		vector<UINT> CraftList = *(CharacterData->GetCraftListAdress());
		
		if (CraftList.empty())
			return BT_FAILURE;
		else
		{
			int CraftItemID = CraftList[0];
			GetBlackBoard()->SetBBData(L"CraftItemID", CraftItemID);
			return BT_SUCCESS;
		}
	}
};

// 다음파밍할 아이템박스가 있는지 여부
class Con_hasNextBox : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		CGameObject* ItemBox = nullptr;
		GetBlackBoard()->FindBBData(L"ExploreItemBox", &ItemBox);

		if (nullptr != ItemBox)
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
};


// ================== 전투 ==================

// 스킬 사용 가능 여부
class Con_IsActive_Skill_Q : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		// 스킬리스트를 불러온다
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		tSkill_Info* skill = CharacterData->GetSkill((UINT)SKILLIDX::Q_1);
		
		int iCurSP = CharacterData->GetStatus()->iSP;

		if (skill->IsAbleUse(iCurSP))
		{
			GetBlackBoard()->SetBBData(L"CurActiveSkill", (int)ER_ActionScript_Character::ER_CHAR_ACT::SKILL_Q);
			return BT_SUCCESS;
		}
		else
			return BT_FAILURE;
	}
};

class Con_IsActive_Skill_W : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		// 스킬리스트를 불러온다
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		tSkill_Info* skill = CharacterData->GetSkill((UINT)SKILLIDX::W_1);

		int iCurSP = CharacterData->GetStatus()->iSP;

		if (skill->IsAbleUse(iCurSP))
		{
			GetBlackBoard()->SetBBData(L"CurActiveSkill", (int)ER_ActionScript_Character::ER_CHAR_ACT::SKILL_W);
			return BT_SUCCESS;
		}
		else
			return BT_FAILURE;
	}
};

class Con_IsActive_Skill_E : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		// 스킬리스트를 불러온다
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		tSkill_Info* skill = CharacterData->GetSkill((UINT)SKILLIDX::E_1);

		int iCurSP = CharacterData->GetStatus()->iSP;

		if (skill->IsAbleUse(iCurSP))
		{
			GetBlackBoard()->SetBBData(L"CurActiveSkill", (int)ER_ActionScript_Character::ER_CHAR_ACT::SKILL_E);
			return BT_SUCCESS;
		}
		else
			return BT_FAILURE;
	}
};

class Con_IsActive_Skill_R : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		// 스킬리스트를 불러온다
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		tSkill_Info* skill = CharacterData->GetSkill((UINT)SKILLIDX::R_1);

		int iCurSP = CharacterData->GetStatus()->iSP;

		if (skill->IsAbleUse(iCurSP))
		{
			GetBlackBoard()->SetBBData(L"CurActiveSkill", (int)ER_ActionScript_Character::ER_CHAR_ACT::SKILL_R);
			return BT_SUCCESS;
		}
		else
			return BT_FAILURE;
	}
};

class Con_IsInSkillRange : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		
		int CurActiveSkill = -1;
		GetBlackBoard()->FindBBData(L"CurActiveSkill", CurActiveSkill);
		
		CurActiveSkill -= 8;

		tSkill_Info* skill = CharacterData->GetSkill(CurActiveSkill);

		Vec3 vTargetPos = ER_GameSystem::GetInst()->GetPlayerCharacter()->Transform()->GetRelativePos();
		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

		vTargetPos.y = 0.f;
		vPos.y = 0.f;

		float fDist = Vec3::Distance(vTargetPos, vPos);
		float fRange = skill->Range();

		if (fDist <= fRange)
		{
			((Root_Node*)GetRootNode())->ClearRunningNode();
			return BT_SUCCESS;
		}
		else
		{
			ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();

			float dist = 1.5f < (fRange - 0.5f) ? 1.5f : (fRange - 0.5f);
			Vec3 vMovePos = vTargetPos + ((vPos - vTargetPos).Normalize() * dist);

			tFSMData FSMData = {};
			FSMData.v4Data = vMovePos;

			Action->Move(FSMData);

			((Root_Node*)GetRootNode())->SetRunningNode(this);
			return BT_RUNNING;
		}
	}
};

// FSM 판단

class Con_IsWAITState : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();

		if (Action->GetCurState() == ER_ActionScript_Character::ER_CHAR_ACT::WAIT)
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
};

class Con_IsMOVEState : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();

		if (Action->GetCurState() == (ER_ActionScript_Character::ER_CHAR_ACT::MOVE))
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
};

class Con_IsFARMINGState : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();

		if (Action->GetCurState() == (ER_ActionScript_Character::ER_CHAR_ACT::FARMING))
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
};

class Con_IsCRAFTState : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();

		if (Action->GetCurState() == (ER_ActionScript_Character::ER_CHAR_ACT::CRAFT))
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
};

class Con_IsRESTState : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();

		if (Action->GetCurState() == (ER_ActionScript_Character::ER_CHAR_ACT::REST))
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
};

class Con_IsATTACKState : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();

		if (Action->GetCurState() == (ER_ActionScript_Character::ER_CHAR_ACT::ATTACK))
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
};

class Con_IsARRIVEState : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();

		if (Action->GetCurState() == (ER_ActionScript_Character::ER_CHAR_ACT::ARRIVE))
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
};

class Con_IsDEADState : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();

		if (Action->GetCurState() == (ER_ActionScript_Character::ER_CHAR_ACT::DEAD))
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
};

class Con_IsSKILL_QState : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();

		if (Action->GetCurState() == (ER_ActionScript_Character::ER_CHAR_ACT::SKILL_Q))
		{
			return BT_SUCCESS;
		}
		else
			return BT_FAILURE;
	}
};

class Con_IsSKILL_WState : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();

		if (Action->GetCurState() == (ER_ActionScript_Character::ER_CHAR_ACT::SKILL_W))
		{
			return BT_SUCCESS;
		}
		else
			return BT_FAILURE;
	}
};

class Con_IsSKILL_EState : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();

		if (Action->GetCurState() == (ER_ActionScript_Character::ER_CHAR_ACT::SKILL_E))
		{
			return BT_SUCCESS;
		}
		else
			return BT_FAILURE;
	}
};

class Con_IsSKILL_RState : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();

		if (Action->GetCurState() == (ER_ActionScript_Character::ER_CHAR_ACT::SKILL_R))
		{
			return BT_SUCCESS;
		}
		else
			return BT_FAILURE;
	}
};

	

// =====================================
// ======[ Action Node : 동작 노드 ]=====
// =====================================


// 아이템 제작 노드
class Act_CraftItem : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();
		
		int CraftItemID = 0;
		GetBlackBoard()->FindBBData(L"CraftItemID", CraftItemID);
		
		tFSMData StateData = {};
		StateData.iData[0] = CraftItemID;
		
		Action->Craft(StateData);

		if (Action->GetCurState() == (ER_ActionScript_Character::ER_CHAR_ACT::CRAFT))
		{
			((Root_Node*)GetRootNode())->ClearRunningNode();
			return BT_SUCCESS;
		}
		else
		{
			// 제작이 불가능한 상태인경우 다음 프레임에서 다시 제작 시도
			((Root_Node*)GetRootNode())->SetRunningNode(this);
			return BT_RUNNING;
		}
	}
};



class Act_SkillLevelUp : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		vector<ER_SKILL*> skillList = CharacterData->GetSkillList();
		UINT* SkillPoint = CharacterData->GetSkillPoint();

		int ResIdx = -1;
		int MinLevel = 5;

		for (int i = 0; i < 4; ++i)
		{
			// 최대레벨인경우 continue;
			if (skillList[i]->iSkillLevel == skillList[i]->iMaxSkillLevel)
				continue;

			// 더 작은 SkillLevel이 있다면 MinLevel과 스킬투자할 Idx 갱신
			if (skillList[i]->iSkillLevel < MinLevel)
			{
				MinLevel = skillList[i]->iSkillLevel;
				ResIdx = i;
			}
		}

		tSkill_Info* Skill_1 = CharacterData->GetSkill(ResIdx);
		tSkill_Info* Skill_2 = CharacterData->GetSkill(ResIdx + 4);

		if (ResIdx != -1)
		{
			Skill_1->iSkillLevel++;
			Skill_2->iSkillLevel++;
			(*SkillPoint) -= 1;
			return BT_SUCCESS;
		}
		else
			return BT_FAILURE;
	}
};

class Act_Farming : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		// 현재 파밍중인 아이템박스 슬롯을 얻어온다.
		CGameObject* ItemBox = nullptr;
		GetBlackBoard()->FindBBData(L"ExploreItemBox", &ItemBox);
		ER_DataScript_ItemBox* ItemBoxData = ItemBox->GetScript<ER_DataScript_ItemBox>();
	
		// 필요 아이템 목록을 받아온다.
		ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();
		unordered_map<UINT, int> FarmingList = CharacterData->GetNeedFarmingList();	// 얕은복사로 원본데이터 갱신되지않음

		


		for (int SlotIdx = 0; SlotIdx < 8; SlotIdx++)
		{
			// 슬롯을 순회하며 슬롯 오브젝트를 얻어온다
			
			CGameObject** CurSlot = &ItemBoxData->GetItemList()[SlotIdx];

			// 아이템 슬롯이 비어있다면 Continue;
			if (nullptr == *CurSlot)
				continue;
			
			ER_DataScript_Item* ItemData = (*CurSlot)->GetScript<ER_DataScript_Item>();
			
			// 슬롯의 아이템정보 확인
			unordered_map<UINT, int>::iterator iter = FarmingList.find(ItemData->GetCode());
			
			// 슬롯의 아이템이 필요아이템 목록에 있는경우 습득함수 동작
			if (iter != FarmingList.end())
			{
				// 습득함수에 현재확인중인 아이템슬롯오브젝트의 주소를 넣어준다.
				CharacterData->AcquireItem(CurSlot);

				// 사용중인 FarmingList 정보 갱신
				if (iter->second == 1)
					FarmingList.erase(iter);
				else
					iter->second--;
			}
		}

		return BT_SUCCESS;
	}
};

class Act_FarmingEnd : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		// 파밍이 종료되면 FSMState를 Wait로 바꾸어 FarmingState를 종료해주고 ExploreItemBox를 갱신해준다.

		// 대기명령
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();
		tFSMData FSMData = {};
		Action->Wait(FSMData);

		if (Action->GetCurState() == (ER_ActionScript_Character::ER_CHAR_ACT::WAIT))
		{
			// ExploreItemBox 갱신
			CGameObject* ItemBox = GetOwner()->GetScript<ER_AIScript>()->GetNextItemBox();
			GetBlackBoard()->SetBBData(L"ExploreItemBox", ItemBox);

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

class Act_FarmingCommand : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		// 블랙보드에서 아이템박스를 얻어와서 파밍명령 실행

		CGameObject* ItemBox = nullptr;
		GetBlackBoard()->FindBBData(L"ExploreItemBox", &ItemBox);
		
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();
		
		tFSMData FSMData = {};
		FSMData.lParam = (DWORD_PTR)ItemBox;
		
		// 파밍명령
		Action->Farming(FSMData);

		// 파밍명령이 성공했는지 상태검사
		if (ER_ActionScript_Character::ER_CHAR_ACT::FARMING == Action->GetCurState())
		{
			((Root_Node*)GetRootNode())->ClearRunningNode();
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

class Act_Skill_Q : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();
		CGameObject* Target = ER_GameSystem::GetInst()->GetPlayerCharacter();

		tFSMData FSMData = {};
		
		FSMData.v4Data = Target->Transform()->GetRelativePos();
		FSMData.lParam = (DWORD_PTR)Target;

		Action->Skill_Q(FSMData);

		if (Action->GetCurState() == (ER_ActionScript_Character::ER_CHAR_ACT::SKILL_Q))
			return BT_SUCCESS;
		else
			return BT_RUNNING;
	}
};

class Act_Skill_W : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();
		CGameObject* Target = ER_GameSystem::GetInst()->GetPlayerCharacter();

		tFSMData FSMData = {};

		FSMData.v4Data = Target->Transform()->GetRelativePos();
		FSMData.lParam = (DWORD_PTR)Target;

		Action->Skill_W(FSMData);

		if (Action->GetCurState() == (ER_ActionScript_Character::ER_CHAR_ACT::SKILL_W))
			return BT_SUCCESS;
		else
			return BT_RUNNING;
	}
};

class Act_Skill_E : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();
		CGameObject* Target = ER_GameSystem::GetInst()->GetPlayerCharacter();

		tFSMData FSMData = {};

		FSMData.v4Data = Target->Transform()->GetRelativePos();
		FSMData.lParam = (DWORD_PTR)Target;

		Action->Skill_E(FSMData);

		if (Action->GetCurState() == (ER_ActionScript_Character::ER_CHAR_ACT::SKILL_E))
			return BT_SUCCESS;
		else
			return BT_RUNNING;
	}
};

class Act_Skill_E_BackDir : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();
		CGameObject* Target = ER_GameSystem::GetInst()->GetPlayerCharacter();

		tFSMData FSMData = {};

		Vec3 vTargetPos = Target->Transform()->GetRelativePos();
		Vec3 vOwnerPos = GetOwner()->Transform()->GetRelativePos();

		// 반대방향
		Vec3 vBackDir = (vTargetPos - vOwnerPos).Normalize();
		Vec3 vSkillPos = vOwnerPos + (vBackDir * 3.f);

		FSMData.v4Data = vSkillPos;
		FSMData.lParam = (DWORD_PTR)Target;

		Action->Skill_E(FSMData);

		if (Action->GetCurState() == (ER_ActionScript_Character::ER_CHAR_ACT::SKILL_E))
			return BT_SUCCESS;
		else
			return BT_RUNNING;
	}
};

class Act_Skill_R : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();
		CGameObject* Target = ER_GameSystem::GetInst()->GetPlayerCharacter();

		tFSMData FSMData = {};

		FSMData.v4Data = Target->Transform()->GetRelativePos();
		FSMData.lParam = (DWORD_PTR)Target;

		Action->Skill_R(FSMData);

		if (Action->GetCurState() == (ER_ActionScript_Character::ER_CHAR_ACT::SKILL_R))
			return BT_SUCCESS;
		else
			return BT_RUNNING;
	}
};

class Act_WaitForSkillActivation : public Task_Node
{
public:
	virtual BT_STATUS Run() override
	{
		ER_ActionScript_Character* Action = GetOwner()->GetScript<ER_ActionScript_Character>();

		int CurActiveSkill = -1;
		GetBlackBoard()->FindBBData(L"CurActiveSkill", CurActiveSkill);
		
		if (Action->GetCurState() == ((ER_ActionScript_Character::ER_CHAR_ACT)CurActiveSkill))
		{
			BTNode* node = this;
			((Root_Node*)GetRootNode())->SetRunningNode(node);
			return BT_RUNNING;
		}
		else
		{
			((Root_Node*)GetRootNode())->ClearRunningNode();
			return BT_SUCCESS;
		}
	}
};



class Act_Attack : public Task_Node
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
		if (Action->GetCurState() == (ER_ActionScript_Character::ER_CHAR_ACT::ATTACK))
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
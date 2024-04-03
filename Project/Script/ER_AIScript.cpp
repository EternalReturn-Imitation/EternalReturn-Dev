#include "pch.h"
#include "ER_AIScript.h"

#include "ER_AIScript_Nodes.cpp""

ER_AIScript::ER_AIScript(SCRIPT_TYPE _type)
	:CScript((UINT)_type)
	, m_Character(nullptr)
	, m_pActionScript(nullptr)
	, m_pBehaviorTree(nullptr)
{
}

ER_AIScript::~ER_AIScript()
{
}

void ER_AIScript::SetBattleStartTime(float _fTime)
{
	if (m_pBehaviorTree)
	{
		BB* BlackBoard = m_pBehaviorTree->GetRootNode()->GetBlackBoard();
		BlackBoard->AddBBData(L"BattleTime", _fTime);
	}
}

Selector_Node* ER_AIScript::SetERCharacterAIBase()
{
	// 행동트리 Component 생성
	m_pBehaviorTree = onew(CBehaviorTree);
	GetOwner()->AddComponent(m_pBehaviorTree);
	m_pBehaviorTree->SetOwner(GetOwner());


	// =========================== [   1   ] ===========================
	// 루트 할당
	Root_Node* root = m_pBehaviorTree->GetRootNode();

	SetBattleStartTime(60.f);

	// [ 생존 판단 ]
	// Seq_CheckArrive : 캐릭터 생존 판단 시퀀스
	Sequence_Node* Seq_CheckArrive = root->AddChild<Sequence_Node>();
	// Con_IsArrive : 캐릭터 생존 판단 노드
	Seq_CheckArrive->AddChild<Con_IsArrive>();

	// Sel_ExecuteAICommand : AI 명령 실행 셀렉터
	Selector_Node* Sel_ExecuteAICommand = root->AddChild<Selector_Node>();

	// Seq_CheckGameTime : GameTime 점검 시퀀스
	Sequence_Node* Seq_CheckGameTime = Sel_ExecuteAICommand->AddChild<Sequence_Node>();

	// Con_IsBeforeTime : 지정한 시간이 지나기 전인지 판단
	Seq_CheckGameTime->AddChild <Con_IsBeforeTime>();

	// Sel_ExploreCommand : 탐색(성장) 관련 명령 셀렉터
	Selector_Node* Sel_ExploreCommand = Seq_CheckGameTime->AddChild<Selector_Node>();

	// =========================== [   2   ] ===========================
	// 성장 ->  Sel_ExploreCommand 하위 우선순위 첫번째
	// 



	// =========================== [   3   ] ===========================








	// Sel_BattleCommand : 전투관련 명령 셀렉터
	Selector_Node* Sel_BattleCommand = Sel_ExecuteAICommand->AddChild<Selector_Node>();

	return Sel_BattleCommand;
}
#include "pch.h"
#include "ER_AIScript.h"

#include "ER_AIScript_Nodes.cpp"
#include "ER_ItemMgr.h"

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

void ER_AIScript::RegistAllItemBox(int _StartIdx)
{
	// 시작 인덱스부터 에어리어 끝-> 처음부터 시작인덱스 전까지
	
	int iAreaIdx = _StartIdx;
	
	// 인자로 받은 인덱스부터 시작해서 끝까지 탐색하고, 처음으로 돌아가 시작인덱스 전까지 작업수행
	do
	{
		// 현재 지역의 아이템박스를 전부 받아온다.
		vector<CGameObject*> AreaItemBox = ER_ItemMgr::GetInst()->GetItemBoxList(iAreaIdx);

		// 아이템박스 순서를 섞어준다
		std::random_device rd;
		std::mt19937 gen(rd());

		std::shuffle(AreaItemBox.begin(), AreaItemBox.end(), gen);

		// 순서대로 탐색 아이템박스 Queue에 집어넣어준다
		for (auto ItemBox : AreaItemBox)
		{
			m_queueItemBox.push(ItemBox);
		}

		// 인덱스 재설정
		// 마지막 지역 Idx에 도달했다면 처음으로 설정
		if (iAreaIdx + 1 == (UINT)LUMIAISLAND::END)
			iAreaIdx = (UINT)LUMIAISLAND::ARCHERY; // 0 번지역
		else
			iAreaIdx++;	// 다음지역으로 인덱스 증가

	} while (iAreaIdx != _StartIdx);


}


CGameObject* ER_AIScript::GetNextItemBox()
{
	CGameObject* ItemBox = nullptr;
	
	if (!m_queueItemBox.empty())
	{
		m_queueItemBox.pop();
		ItemBox = m_queueItemBox.front();
	}

	return ItemBox;
}


Selector_Node* ER_AIScript::SetERCharacterAIBase()
{
	// 행동트리 Component 생성
	m_pBehaviorTree = onew(CBehaviorTree);
	GetOwner()->AddComponent(m_pBehaviorTree);
	m_pBehaviorTree->SetOwner(GetOwner());

	// 공용 BlackBoard 설정
	BB* BlackBoard = m_pBehaviorTree->GetRootNode()->GetBlackBoard();
	int CraftItemID = -1;
	CGameObject* ItemBox = m_queueItemBox.front();
	BlackBoard->AddBBData(L"CraftItemID", CraftItemID);
	BlackBoard->AddBBData(L"ExploreItemBox", ItemBox);
	BlackBoard->AddBBData(L"CurActiveSkill", -1);
	BlackBoard->AddBBData(L"CheckTime", 0.f);

	// =========================== [   1   ] ===========================
	// 루트 할당
	Root_Node* root = m_pBehaviorTree->GetRootNode();

	// [ 생존 판단 ]
	// Seq_CheckArrive : 캐릭터 생존 판단 시퀀스
	Selector_Node* Sel_CheckArrive = root->AddChild<Selector_Node>();
	// Con_IsDEADState : 캐릭터가 사망상태인지
	Sel_CheckArrive->AddChild<Con_IsDEADState>();

	// Sel_ExecuteAICommand : AI 명령 실행 셀렉터
	Selector_Node* Sel_ExecuteAICommand = Sel_CheckArrive->AddChild<Selector_Node>();

	// Seq_CheckGameTime : GameTime 점검 시퀀스
	Sequence_Node* Seq_CheckGameTime = Sel_ExecuteAICommand->AddChild<Sequence_Node>();

	// Con_IsBeforeTime : 지정한 시간이 지나기 전인지 판단
	Seq_CheckGameTime->AddChild <Con_IsBeforeTime>();

	// Sel_ExploreCommand : 탐색(성장) 관련 명령 셀렉터
	Selector_Node* Sel_ExploreCommand = Seq_CheckGameTime->AddChild<Selector_Node>();

	// =========================== [   스킬성장   ] ===========================
	Sequence_Node* Seq_SkillUp = Sel_ExploreCommand->AddChild<Sequence_Node>();
	
	Seq_SkillUp->AddChild<Con_IsHaveSkillPoint>();	// 스킬 포인트 검사
	Seq_SkillUp->AddChild<Act_SkillLevelUp>();		// 스킬 포인트 투자

	// =========================== [   제작   ] ===========================
	Selector_Node* Sel_Craft = Sel_ExploreCommand->AddChild<Selector_Node>();
	// 현재 아이템이 제작중인지 : 제작중이라면 이번 명령을 마무리한다.
	Sel_Craft->AddChild<Con_IsCRAFTState>();
	
	// 제작중이 아니라면 아이템 제작 진행
	Sequence_Node* Seq_Craft = Sel_Craft->AddChild<Sequence_Node>();

	// 제작 가능한 아이템이 있는지 : 있다면 블랙보드에 제작할 아이템 아이디를 저장하고 그 아이템을 제작
	Seq_Craft->AddChild<Con_IsCanCraftItem>();
	Seq_Craft->AddChild<Act_CraftItem>();

	// =========================== [   파밍   ] ===========================
	Selector_Node* Sel_Farming = Sel_ExploreCommand->AddChild<Selector_Node>();

	// 파밍상태인지 확인후, 파밍작업 수행. 종료 후 아이템박스탐색 큐 갱신
	Sequence_Node* Seq_Farming = Sel_Farming->AddChild<Sequence_Node>();
	Seq_Farming->AddChild<Con_IsFARMINGState>();
	Seq_Farming->AddChild<Act_Farming>();
	Seq_Farming->AddChild<Act_FarmingEnd>();

	// 파밍상태가 아니라면 탐색중인 아이템박스로 이동한다
	Sequence_Node* Seq_Explore = Sel_Farming->AddChild<Sequence_Node>();
	Seq_Explore->AddChild<Con_hasNextBox>();
	Seq_Explore->AddChild<Act_FarmingCommand>();



	// Sel_BattleCommand : 전투관련 명령 셀렉터
	Seq_CheckGameTime = Sel_ExecuteAICommand->AddChild<Sequence_Node>();
	Seq_CheckGameTime->AddChild<Con_IsAfterTime>();
	Selector_Node* Sel_BattleCommand = Seq_CheckGameTime->AddChild<Selector_Node>();

	return Sel_BattleCommand;
}
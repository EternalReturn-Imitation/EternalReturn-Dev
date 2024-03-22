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
	, m_AccessGrade(eAccessGrade::BASIC)
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
	// 기본 대기상태
	if (IsAbleChange(eAccessGrade::BASIC))
		ChangeState(ER_CHAR_ACT::WAIT);
}
void ER_ActionScript_Character::Move(tFSMData& _Data)
{
	// 이동가능 상태 판단
	if (IsAbleChange(eAccessGrade::BASIC))
	{
		if (m_iCurState != (UINT)ER_CHAR_ACT::MOVE)
		{
			STATEDATA_SET(MOVE, _Data);
			ChangeState(ER_CHAR_ACT::MOVE);				// 이동중이 아니었다면 이동상태 변환
		}
		else
			FindPath()->FindPath(_Data.v4Data);			// 이동중이었다면 목표지점 갱신
	}
}
void ER_ActionScript_Character::Rest(tFSMData& _Data)
{
	if (IsAbleChange(eAccessGrade::BASIC))
		ChangeState(ER_CHAR_ACT::REST);
}
void ER_ActionScript_Character::Attack(tFSMData& _Data)
{
	/*
	[ATTACK]
	bData[0]	: 공격동작 진행중인지 여부
	bData[1]	: Battle Event 실행 여부
	bData[2]	: 다음 타겟 지정 여부
	iData[0]	: 타격지점 애니메이션 프레임 = Hit Frame
	lParam		: 타겟 오브젝트
	RParam		: 타겟 예정 오브젝트

	[MOVE]
	bData[0]	: 타겟 추적 여부
	fData		: 공격 가능 거리
	iData[0]	: 타겟 타입 : 1 - 공격대상, 2 - 아이템박스
	v4Data		: 목표 이동 좌표
	*/

	// 타겟 오브젝트 사망여부 판단
	if (((CGameObject*)_Data.lParam)->GetScript<ER_DataScript_Character>()->IsDeadState())
		return;

	// 이전 공격상태의 데이터를 받아온다
	tFSMData PrevData = STATEDATA_GET(ATTACK);

	//  현재 공격중이었는가
	if (PrevData.bData[0])
	{
		_Data.bData[0]	= PrevData.bData[0];	// 공격 상태 유지
		_Data.bData[1]	= PrevData.bData[1];	// Hit 판정 유지
		_Data.fData		= PrevData.fData;		// 공격 가능 거리 유지
		_Data.iData[0]  = PrevData.iData[0];		// HitFrame 유지

		// 새로운 타겟을 지정했는가
		if (!(PrevData.lParam) && _Data.lParam != PrevData.lParam)
		{
			_Data.RParam	= _Data.lParam;			// 새로운 타겟을 다음 타겟으로 지정
			_Data.lParam	= PrevData.lParam;		// 현재타겟 유지
			_Data.bData[2]	= true;					// 다음타겟 지정되어있음
		}

		STATEDATA_SET(ATTACK, _Data);				// 데이터 업데이트
		return;										// 상태를 변경하지 않고 종료
	}
	// 타겟 추적중이다
	else
	{
		_Data.fData = GetStatus()->fAtkRange;		// 일반공격 가능 거리;

		// 공격 사정거리 범위 판단
		if (IsInRange((CGameObject*)_Data.lParam, _Data.fData))
		{
			STATEDATA_SET(ATTACK, _Data);			// 입력받은 데이터로 세팅
			SetStateGrade(eAccessGrade::BASIC);	// 모션중 취소가능
			ChangeState(ER_CHAR_ACT::ATTACK);
		}
		else
		{
			tFSMData MoveData = {};
			MoveData.lParam		= _Data.lParam;		// 타겟
			MoveData.bData[0]	= true;				// 타겟 추적중
			MoveData.fData		= _Data.fData;		// 공격 가능 거리
			MoveData.iData[0]	= 1;				// 타겟 타입 : 1 공격대상
			MoveData.v4Data		= ((CGameObject*)_Data.lParam)->Transform()->GetRelativePos();
			Move(MoveData);
		}
	}

}
void ER_ActionScript_Character::Farming(tFSMData& _Data)
{
	/*
	명령함수
	거리판단 후 추적명령
	
	Player :
	파밍 명령

	Enter -> UI호출
	Update -> UI 렌더하면서 UIObject와 상호작용 
	Exit ->	UI를 호출해제한다.

	AI (Player가 아님):

	Enter -> 아무동작하지않음
	Update -> Time 2초 소모
	: 
	
	필요루트중 제작가능 아이템이 있다면 제작 -> Craft 함수 호출
	lParam에 상자Object저장하고 Craft 끝나고 다시 파밍호출			- 불가할때까지 반복

	제작 아이템이 없는경우 필요아이템 확인
	공간이 있다 -> 습득
	공간이 없다 -> AI스크립트의 타겟 상자에서 지움.


	[FARMING]
	bData[0]	: 파밍중인지 여부
	fData		: 파밍 가능 거리
	lParam		: 타겟박스 오브젝트
	
	[MOVE]
	bData[0]	: 타겟 추적 여부
	fData		: 파밍 가능 거리
	iData[0]	: 타겟 타입 : 1 - 공격대상, 2 - 아이템박스
	v4Data		: 목표 이동 좌표
	*/

	// 이전 공격상태의 데이터를 받아온다
	tFSMData PrevData = STATEDATA_GET(FARMING);

	//  현재 파밍중이고 같은박스에 파밍명령을 내렸는가
	if (PrevData.bData[0] && PrevData.lParam == _Data.lParam)
		return;						// 상태를 변경하지 않고 종료
	else
	{
		float fFarmingDist = 2.f;								// 파밍가능 거리 2거리
		
			// 아이템박스가 접근 가능 거리에있는지
		if (IsInRange((CGameObject*)_Data.lParam, fFarmingDist))
		{
			STATEDATA_SET(FARMING, _Data);
			SetStateGrade(eAccessGrade::BASIC);	// 모션중 취소가능
			ChangeState(ER_CHAR_ACT::FARMING);
		}
		else
		{
			tFSMData MoveData	= {};
			MoveData.bData[0]	= true;				// 파밍박스 추적중
			MoveData.iData[0]   = 2;				// 타겟 타입 : 2 - 아이템박스
			MoveData.v4Data = ((CGameObject*)_Data.lParam)->Transform()->GetRelativePos();
			Move(MoveData);
		}
	}
}
void ER_ActionScript_Character::Craft(tFSMData& _Data)
{
	/*
	명령을 실행할 때 제작할 아이템을
	
	제작가능 아이템은 아이템 습득 함수때

	ER_CMB_SLOT 으로 받아오면 : int 1 슬롯번호, int 2 슬롯번호, UINT 제작할 아이템
	넘겨줘서
	애니메이션 종료되면 슬롯 1, 슬롯 2 아이템 Destory해주고,  nullptr로 슬롯 비워주고

	CharacterData 아이템 습득 함수 호출
	

	[CRAFT]
	bData[0]	: 제작중 여부
	iData[0]	: 제작할 아이템 코드
	iData[1]	: 재료1 아이템 슬롯 번호
	iData[2]	: 재료2 아이템 슬롯 번호
	*/

	// 이전 공격상태의 데이터를 받아온다
	tFSMData PrevData = STATEDATA_GET(CRAFT);

	//  현재 아이템 제작중인가
	if (PrevData.bData[0])
		return;						// 상태를 변경하지 않고 종료
	else
	{
		STATEDATA_SET(CRAFT, _Data);
		SetStateGrade(eAccessGrade::BASIC);	// 모션중 취소가능
		ChangeState(ER_CHAR_ACT::CRAFT);
	};
}
void ER_ActionScript_Character::Dead(tFSMData& _Data)
{
	ChangeState(ER_CHAR_ACT::DEAD,eAccessGrade::UTMOST);
	m_Data->SetGameDead();	// 캐릭터 사망상태 처리 : 타겟지정불가, tick 수행하지 않음
}

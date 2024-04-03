#pragma once
#include <Engine\CScript.h>
#include <Engine\CSound.h>
#include <Engine\ptr.h>

class FSM;
class FSMState;
class ER_DataScript_Character;

class ER_ActionScript_Character :
    public CScript
{
public:
    enum class eCharacterActionState
    {
        WAIT,       // 대기
        MOVE,       // 이동
        FARMING,    // 파밍
        CRAFT,      // 제작
        REST,       // 휴식
        ATTACK,     // 공격
        ARRIVE,     // 생성
        DEAD,       // 사망
        SKILL_Q,    // Q
        SKILL_W,    // W
        SKILL_E,    // E
        SKILL_R,    // R
        END,
    }typedef ER_CHAR_ACT;

    enum class eAccessGrade
    {
        BASIC,          // 일반 등급 접근
        ADVANCED,       // 상위 등급 접근
        UTMOST,         // 최고 등급 접근
    };


protected:
    FSM*                        FSMContext;
    FSMState*                   StateList[(UINT)ER_CHAR_ACT::END];  // State List
    UINT                        m_iPrevState;                       // Prev State
    UINT                        m_iCurState;                        // Cur State
    ER_DataScript_Character*    m_Data;                             // Cur Character Data
    eAccessGrade                m_AccessGrade;                      // 동작 접근 가능 등급

    // Create State
    virtual FSMState* CreateWait() = 0;
    virtual FSMState* CreateArrive() = 0;
    virtual FSMState* CreateDead() = 0;

    virtual FSMState* CreateAttack() = 0;
    virtual FSMState* CreateMove() = 0;
    virtual FSMState* CreateFarming() = 0;
    virtual FSMState* CreateCraft() = 0;
    virtual FSMState* CreateRest() = 0;
    
    virtual FSMState* CreateSkill_Q() = 0;
    virtual FSMState* CreateSkill_W() = 0;
    virtual FSMState* CreateSkill_E() = 0;
    virtual FSMState* CreateSkill_R() = 0;

    virtual bool SoundLoad() = 0;

    // [ Status Func ]
    tIngame_Stats* GetStatus();                                     // 캐릭터 스탯 확인
    tStatus_Effect* GetStatusEffect();                              // 캐릭터 상태효과 확인
    ER_DataScript_Character* GetCharacterData();                    // 캐릭터 데이터스크립트 확인

    bool IsSkillOn(SKILLIDX _idx);                                  // 지속스킬 동작 판단

    // [ Transform Func ]
    Vec3 GetFocusPoint();                                           // 타겟 좌표 확인
    Vec3 GetFocusDir();                                             // 타겟 방향 확인
    Vec3 GetClearPoint(const Vec3& vDir, float dist);               // 이동 가능 지점 확인
    float GetClearDistance(const Vec3& vDir, float dist);           // 이동 가능 거리 확인
    float GetClearDistanceByWall(const Vec3& vDir, float dist);     // 벽까지 이동가능한 거리 확인
    Vec3 SetRotationToTarget(const Vec3& vTarget);                  // 타겟 방향 회전

    bool IsInRange(CGameObject* Target, float _fRange);             // 거리범위 확인
    bool IsInRangeWithAngle(CGameObject* _Owner, CGameObject* _Target, float _fRange, float _degree);
    Vec3 GetProjSpawnPos(DWORD_PTR _Target);

    // [ State Manage ]
    void StateInit();                                                                // 상태 초기화 세팅
    bool ChangeState(ER_CHAR_ACT _state, eAccessGrade _Grade = eAccessGrade::BASIC); // 상태 변경
    void SetStateGrade(eAccessGrade _Grade) { m_AccessGrade = _Grade; }              // 상태 접근 가능 등급
    bool IsAbleChange(eAccessGrade _Grade);                                          // 접근 등급 비교


    // [ Sound ]
    virtual void PlaySound(UINT _SoundKey) {}

    // [ Player Check ]
    bool IsPlayer();
    bool IsCharacter(CGameObject* _Obj);
    bool IsDead(CGameObject* _Obj);


public:
    virtual void begin() override;
    virtual void tick() override;

    ER_CHAR_ACT GetCurState() { return (ER_CHAR_ACT)m_iCurState; }
    bool IsThisState(ER_CHAR_ACT _State) { return _State == (ER_CHAR_ACT)m_iCurState; }

    // 공통 함수
    virtual void Attack(tFSMData& _Data);       // 기본공격
    virtual void Wait(tFSMData& _Data);         // 대기
    virtual void Move(tFSMData& _Data);         // 이동
    virtual void Farming(tFSMData& _Data);      // 아이템파밍
    virtual void Craft(tFSMData& _Data);        // 제작
    virtual void Rest(tFSMData& _Data);         // 휴식
    virtual void Dead(tFSMData& _Data);         // 사망
    
    // 개별 구현 함수
    virtual void Skill_Q(tFSMData& _Data) = 0;  // Q Skill
    virtual void Skill_W(tFSMData& _Data) = 0;  // W Skill
    virtual void Skill_E(tFSMData& _Data) = 0;  // E Skill
    virtual void Skill_R(tFSMData& _Data) = 0;  // R Skill

public:
    ER_ActionScript_Character(SCRIPT_TYPE _type);
    ~ER_ActionScript_Character();

    CLONE_DISABLE(ER_ActionScript_Character);
};
#pragma once
#include <Engine\CScript.h>

class FSM;
class FSMState;
class ER_DataScript_Character;

class ER_ActionScript_Character :
    public CScript
{
public:
    enum eCharacterActionState
    {
        WAIT,       // 대기
        MOVE,       // 이동
        CRAFT,      // 제작
        REST,       // 휴식
        ATTACK,     // 공격
        ARRIVE,     // 생성
        DEATH,      // 사망
        SKILL_Q,    // Q
        SKILL_W,    // W
        SKILL_E,    // E
        SKILL_R,    // R
        END,
    }typedef ER_CHAR_ACT;

    enum bAbleChange
    {
        COMMON,     // 일반명령
        ABSOUTE,    // 캔슬기
        DISABLE,    // 변경불가
    };


protected:
    FSM* FSMContext;
    FSMState* StateList[ER_CHAR_ACT::END];  // State List
    UINT m_iPrevState;                      // Prev State
    UINT m_iCurState;                       // Cur State
    ER_DataScript_Character* m_Data;        // Cur Character Data

    bAbleChange m_bAbleChange;              // 동작 변경 가능 여부

    // FSMState에 Delegate를 생성해서 연결해주는 함수
    virtual FSMState* CreateWait() = 0;
    virtual FSMState* CreateMove() = 0;
    virtual FSMState* CreateCraft() = 0;
    virtual FSMState* CreateRest() = 0;
    virtual FSMState* CreateAttack() = 0;
    virtual FSMState* CreateArrive() = 0;
    virtual FSMState* CreateDeath() = 0;
    virtual FSMState* CreateSkill_Q() = 0;
    virtual FSMState* CreateSkill_W() = 0;
    virtual FSMState* CreateSkill_E() = 0;
    virtual FSMState* CreateSkill_R() = 0;

protected:
    Vec3 GetFocusPoint();                // 타겟 지점
    Vec3 GetClearPoint(const Vec3& vDir, float dist);
    float GetClearDistance(const Vec3& vDir, float dist);
    float GetClearDistanceByWall(const Vec3& vDir, float dist);
    Vec3 SetRotationToTarget(const Vec3& vTarget);
    Vec3 GetFocusPoint();


public:
    virtual void begin() override;
    virtual void tick() override;

    // 동작 명령 함수. FSM 변경 명령 및 추가 동작 구현
    virtual void Attack(tFSMData& _Data) = 0;   // 기본공격
    virtual void Wait(tFSMData& _Data);         // 대기
    virtual void Move(tFSMData& _Data);         // 이동
    virtual void Craft(tFSMData& _Data) {}      // 제작
    virtual void Rest(tFSMData& _Data) {}       // 휴식
    virtual void Skill_Q(tFSMData& _Data) = 0;  // Q Skill
    virtual void Skill_W(tFSMData& _Data) = 0;  // W Skill
    virtual void Skill_E(tFSMData& _Data) = 0;  // E Skill
    virtual void Skill_R(tFSMData& _Data) = 0;  // R Skill

    void StateInit();

    bool ChangeState(ER_CHAR_ACT _state, bAbleChange _Grade = bAbleChange::COMMON);
    
    void SetAbleToCancle(bAbleChange _Grade) { m_bAbleChange = _Grade; }
    bool IsAbleChange(bAbleChange _Grade);

public:
    ER_ActionScript_Character(SCRIPT_TYPE _type);
    ~ER_ActionScript_Character();

    CLONE_DISABLE(ER_ActionScript_Character);
};
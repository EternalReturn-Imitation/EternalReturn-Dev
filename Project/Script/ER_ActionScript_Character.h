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
    UINT m_iCurState;                       // Cur State
    ER_DataScript_Character* m_Data;        // Cur Character Data

    bAbleChange m_bAbleChange;              // 동작 변경 가능 여부

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

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void Attack(CGameObject* _EnemyObj) = 0;                // 기본공격
    virtual void Wait();                                            // 대기
    virtual void Move(CGameObject* _Target, Vec3 _DestPos);         // 이동
    virtual void Craft() {}                                         // 제작
    virtual void Rest() {}                                          // 휴식
    virtual void Skill_Q(CGameObject* _Target, Vec3 _DestPos) = 0;  // Q Skill
    virtual void Skill_W(CGameObject* _Target, Vec3 _DestPos) = 0;  // W Skill
    virtual void Skill_E(CGameObject* _Target, Vec3 _DestPos) = 0;  // E Skill
    virtual void Skill_R(CGameObject* _Target, Vec3 _DestPos) = 0;  // R Skill

    void StateInit();

    void ChangeState(ER_CHAR_ACT _state);
    
    void SetAbleToCancle(bAbleChange _bAble) { m_bAbleChange = _bAble; }
    bool IsAbleChange(bAbleChange _able);

public:
    ER_ActionScript_Character(SCRIPT_TYPE _type);
    ~ER_ActionScript_Character();

    CLONE_DISABLE(ER_ActionScript_Character);
};
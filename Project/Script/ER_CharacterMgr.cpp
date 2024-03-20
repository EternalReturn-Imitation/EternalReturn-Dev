#include "pch.h"
#include "ER_CharacterMgr.h"

#include "ER_PlayerScript.h"
#include "ER_GameSystem.h"
#include "ER_PlayerScript_Range.h"

ER_CharacterMgr::ER_CharacterMgr()
{
}

ER_CharacterMgr::~ER_CharacterMgr()
{
    Safe_Del_Map(m_mapCharacters);
}

void ER_CharacterMgr::init()
{
    // 캐릭터 데이터 로딩
    Load();
}

CGameObject* ER_CharacterMgr::SpawnCharacter(const wstring& _key)
{
    CGameObject* Character = new CGameObject(*m_mapCharacters.find(_key)->second);
    AddComponents(Character, _COLLIDER3D | _FINDPATH);

    Character->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);

    Character->Transform()->SetRelativeScale(Vec3(1.1f, 1.1f, 1.1f));
    Character->Collider3D()->SetOffsetScale(Vec3(1.0f, 2.0f, 1.0f));
    Character->Collider3D()->SetOffsetPos(Vec3(0.f, 1.0f, 0.f));


    return Character;
}

CGameObject* ER_CharacterMgr::SpawnCharacter_Player(const wstring& _key, Vec3 _Pos)
{
    CGameObject* Player = SpawnCharacter(_key);
    
    // Player Script

    Player->AddComponent(new ER_PlayerScript);


    // SkillArea
    // CGameObject* SkillArea = new CGameObject;
    // SkillArea->AddComponent(new ER_PlayerScript_Range);
    // SkillArea->GetScript<ER_PlayerScript_Range>()->init();
    // 
    // SkillArea->GetScript<ER_PlayerScript_Range>()->SetOwnerObj(Player);

    // CurLevel Spawn
    SpawnGameObject(Player, _Pos, L"Player");
    // SpawnGameObject(SkillArea, _Pos, L"InGameUI");

    //게임시스템에 플레이어의 캐릭터를 할당.
    ER_GameSystem::GetInst()->GetInst()->SetPlayerCharacter(Player);

    return Player;
}

CGameObject* ER_CharacterMgr::SpawnCharacter_Enemy(const wstring& _key, Vec3 _Pos)
{
    CGameObject* Enemy = SpawnCharacter(_key);
    
    // CurLevel Spawn
    SpawnGameObject(Enemy, _Pos, L"Character");

    return Enemy;
}
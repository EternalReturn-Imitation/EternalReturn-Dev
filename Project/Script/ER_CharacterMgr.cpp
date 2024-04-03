#include "pch.h"
#include "ER_CharacterMgr.h"

#include "ER_PlayerScript.h"
#include "ER_GameSystem.h"

#include "ER_UIMgr.h"
#include "ER_UIScript_TrackingStatusBar.h"
#include "ER_DataScript_Character.h"

#include "ER_AIScript.h"
#include "ER_AIScript_Aya.h"
#include "ER_AIScript_Hyunwoo.h"
#include "ER_AIScript_Rio.h"
#include "ER_AIScript_Yuki.h"

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
    CGameObject* Character = m_mapCharacters.find(_key)->second->Clone();
    AddComponents(Character, _COLLIDER3D | _FINDPATH);
    Character->GetScript<ER_DataScript_Character>()->begin();

    Character->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);

    Character->Transform()->SetRelativeScale(Vec3(1.1f, 1.1f, 1.1f));
    Character->Collider3D()->SetOffsetScale(Vec3(1.0f, 4.0f, 1.0f));
    Character->Collider3D()->SetOffsetPos(Vec3(0.f, 0.5f, 0.f));
    ER_UIScript_TrackingStatusBar* StatusBar = new ER_UIScript_TrackingStatusBar;
    Character->AddComponent(StatusBar);
    StatusBar->init(Character);

    ER_GameSystem::GetInst()->RegistCurLevelCharacter(Character);

    return Character;
}

CGameObject* ER_CharacterMgr::SpawnCharacter_Player(const wstring& _key, Vec3 _Pos)
{
    CGameObject* Player = SpawnCharacter(_key);
    
    ER_PlayerScript* PlayerScript = new ER_PlayerScript;
    // Player Script
    Player->AddComponent(PlayerScript);

    // CurLevel Spawn
    SpawnGameObject(Player, _Pos, L"Player");

    //게임시스템에 플레이어의 캐릭터를 할당.
    ER_GameSystem::GetInst()->GetInst()->SetPlayerCharacter(Player);
    ER_UIMgr::GetInst()->RegistPlayerCharacetr();

    return Player;
}

CGameObject* ER_CharacterMgr::SpawnCharacter_Enemy(const wstring& _key, Vec3 _Pos)
{
    CGameObject* Enemy = SpawnCharacter(_key);

    // AI Script 장착
    wstring AIScript = L"ER_AIScript_";
    AIScript += _key;
    
    ER_AIScript* Ai = (ER_AIScript*)CScriptMgr::GetScript(AIScript);
    Enemy->AddComponent(Ai);

    // CurLevel Spawn
    SpawnGameObject(Enemy, _Pos, L"Character");

    Ai->init();

    return Enemy;
}
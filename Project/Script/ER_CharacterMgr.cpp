#include "pch.h"
#include "ER_CharacterMgr.h"

#include "ER_PlayerScript.h"
#include "ER_GameSystem.h"

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

CGameObject* ER_CharacterMgr::GetCharacter(const wstring& _key)
{
    CGameObject* Character = new CGameObject(*m_mapCharacters.find(_key)->second);
    Character->AddComponent(new ER_PlayerScript);
    AddComponents(Character, _COLLIDER3D | _FINDPATH);

    Character->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    Character->Collider3D()->SetOffsetScale(Vec3(1.0f, 2.0f, 1.0f));
    Character->Collider3D()->SetOffsetPos(Vec3(0.f, 1.0f, 0.f));
    Character->Transform()->SetRelativeScale(Vec3(1.1f, 1.1f, 1.1f));

    //게임시스템에 플레이어의 캐릭터를 할당.
    ER_GameSystem::GetInst()->GetInst()->SetPlayerCharacter(Character);

    return Character;
}

CGameObject* ER_CharacterMgr::GetEnemyCharacter(const wstring& _key)
{
    CGameObject* Character = new CGameObject(*m_mapCharacters.find(_key)->second);
    AddComponents(Character, _COLLIDER3D | _FINDPATH);

    Character->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    Character->Collider3D()->SetOffsetScale(Vec3(1.0f, 2.0f, 1.0f));
    Character->Collider3D()->SetOffsetPos(Vec3(0.f, 1.0f, 0.f));
    Character->Transform()->SetRelativeScale(Vec3(1.1f, 1.1f, 1.1f));
    
    return Character;
}

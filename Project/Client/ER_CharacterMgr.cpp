#include "pch.h"
#include "ER_CharacterMgr.h"

#include "ImGuiMgr.h"
#include "CharacterDataUI.h"

#include <Script\ER_PlayerScript.h>

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

    ((CharacterDataUI*)ImGuiMgr::GetInst()->FindUI("##CharacterDataUI"))->RegistCharacters();
}

CGameObject* ER_CharacterMgr::GetCharacter(const wstring& _key)
{
    CGameObject* Character = new CGameObject(*m_mapCharacters.find(_key)->second);
    Character->AddComponent(new ER_PlayerScript);
    Character->AddComponent(new CFindPath);

    Character->Transform()->SetRelativeScale(Vec3(1.1f, 1.1f, 1.1f));

    return Character;
}

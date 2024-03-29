#include "pch.h"
#include <Engine\CResMgr.h>

#include "ER_GameSystem.h"
#include "ER_DataScript_Character.h"

#include "ER_ActionScript_Rio.h"
#include "ER_ActionScript_Yuki.h"
#include "ER_ActionScript_Aya.h"
#include "ER_ActionScript_Hyunwoo.h"
#include "ER_ActionScript_Jackie.h"

#define SOUNDLOAD(Key,FileName) m_pSounds[(UINT)CharacterSound::Key] = CResMgr::GetInst()->FindRes<CSound>(FileName)

bool ER_ActionScript_Rio::SoundLoad()
{
    SOUNDLOAD(LONG_NORMAL_ATTACK, L"Rio_LongBow_NormalAttack_01.wav");
    SOUNDLOAD(SHORT_NORMAL_ATTACK, L"Rio_ShortBow_NormalAttack_01.wav");
    SOUNDLOAD(CRAFT_SOUND, L"chracter_Craft_Tool_8sec.wav");
    return true;
}


bool ER_ActionScript_Yuki::SoundLoad()
{
    SOUNDLOAD(CRAFT_SOUND, L"chracter_Craft_Tool_8sec.wav");
    return true;
}


bool ER_ActionScript_Hyunwoo::SoundLoad()
{
    SOUNDLOAD(CRAFT_SOUND, L"chracter_Craft_Tool_8sec.wav");
    return true;
}


bool ER_ActionScript_Jackie::SoundLoad()
{
    SOUNDLOAD(CRAFT_SOUND, L"chracter_Craft_Tool_8sec.wav");
    return true;
}



bool ER_ActionScript_Aya::SoundLoad()
{
    SOUNDLOAD(CRAFT_SOUND, L"chracter_Craft_Tool_8sec.wav");
    return true;
}

void ER_ActionScript_Rio::PlaySound(UINT _SoundKey)
{
    Vec3 PlayerPos = ER_GameSystem::GetInst()->GetPlayerCharacter()->Transform()->GetRelativePos();
    Vec3 CharacterPos = Transform()->GetRelativePos();
    float ListenDist = Vec3::Distance(PlayerPos, CharacterPos);

    if(ListenDist < 18.f)
        m_pSounds[_SoundKey]->Play(1, 0.5, true);
}

void ER_ActionScript_Yuki::PlaySound(UINT _SoundKey)
{
    Vec3 PlayerPos = ER_GameSystem::GetInst()->GetPlayerCharacter()->Transform()->GetRelativePos();
    Vec3 CharacterPos = Transform()->GetRelativePos();
    float ListenDist = Vec3::Distance(PlayerPos, CharacterPos);

    if (ListenDist < 18.f)
        m_pSounds[_SoundKey]->Play(1, 0.5, true);
}

void ER_ActionScript_Hyunwoo::PlaySound(UINT _SoundKey)
{
    Vec3 PlayerPos = ER_GameSystem::GetInst()->GetPlayerCharacter()->Transform()->GetRelativePos();
    Vec3 CharacterPos = Transform()->GetRelativePos();
    float ListenDist = Vec3::Distance(PlayerPos, CharacterPos);

    if (ListenDist < 18.f)
        m_pSounds[_SoundKey]->Play(1, 0.5, true);
}

void ER_ActionScript_Jackie::PlaySound(UINT _SoundKey)
{
    Vec3 PlayerPos = ER_GameSystem::GetInst()->GetPlayerCharacter()->Transform()->GetRelativePos();
    Vec3 CharacterPos = Transform()->GetRelativePos();
    float ListenDist = Vec3::Distance(PlayerPos, CharacterPos);

    if (ListenDist < 18.f)
        m_pSounds[_SoundKey]->Play(1, 0.5, true);
}

void ER_ActionScript_Aya::PlaySound(UINT _SoundKey)
{
    Vec3 PlayerPos = ER_GameSystem::GetInst()->GetPlayerCharacter()->Transform()->GetRelativePos();
    Vec3 CharacterPos = Transform()->GetRelativePos();
    float ListenDist = Vec3::Distance(PlayerPos, CharacterPos);

    if (ListenDist < 18.f)
        m_pSounds[_SoundKey]->Play(1, 0.5, true);
}
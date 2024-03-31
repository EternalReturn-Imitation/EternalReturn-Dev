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
    SOUNDLOAD(ATTACK_NORMAL1, L"attackTwoHandSword_r1.wav");
    SOUNDLOAD(ATTACK_NORMAL2, L"attackTwoHandSword_r2.wav");
    SOUNDLOAD(HITSOUND, L"Yuki_Passive_Hit_r2.wav");
    SOUNDLOAD(SKILLQ_MOTION, L"Yuki_Skill01_Active.wav");
    SOUNDLOAD(SKILLQ_ATTACK, L"Yuki_Skill01_Attack.wav");
    SOUNDLOAD(SKILLQ_HIT1, L"Yuki_Skill01_Hit_r1.wav");
    SOUNDLOAD(SKILLQ_HIT2, L"Yuki_Skill01_Hit_r2.wav");
    SOUNDLOAD(SKILLW_MOTION, L"Yuki_Skill02_Active.wav");
    SOUNDLOAD(SKILLE_MOTION, L"Yuki_Skill03_Move.wav");
    SOUNDLOAD(SKILLE_ATTACK, L"Yuki_Skill03_Attack.wav");
    SOUNDLOAD(SKILLE_HIT1, L"Yuki_Skill03_Hit_r1.wav");
    SOUNDLOAD(SKILLE_HIT2, L"Yuki_Skill03_Hit_r2.wav");
    SOUNDLOAD(SKILLR_ACTIVE, L"Yuki_Skill04_Active.wav");
    SOUNDLOAD(SKILLR_ATTACK, L"Yuki_Skill04_Attack.wav");
    SOUNDLOAD(SKILLR_HIT, L"Yuki_Skill04_Hit.wav");
    SOUNDLOAD(SKILLR_END, L"Yuki_Skill04_End.wav");

    return true;
}


bool ER_ActionScript_Hyunwoo::SoundLoad()
{
    SOUNDLOAD(CRAFT_SOUND, L"chracter_Craft_Tool_8sec.wav");
    SOUNDLOAD(ATTACK_NORMAL1, L"attackGlove_Normal01.wav");
    SOUNDLOAD(ATTACK_NORMAL2, L"attackGlove_Normal02.wav");
    SOUNDLOAD(HITSOUND, L"attackGlove_Normal_Hit_P.wav");
    SOUNDLOAD(SKILLQ_MOTION, L"hyunwoo_Skill01_Attack_v1.wav");
    SOUNDLOAD(SKILLQ_HIT, L"hyunwoo_Skill01_Hit.wav");
    SOUNDLOAD(SKILLW, L"hyunwoo_Skill02_Activation.wav");
    SOUNDLOAD(SKILLE_SLIDE, L"hyunwoo_Skill03_Slide.wav");
    SOUNDLOAD(SKILLE_HIT, L"hyunwoo_Skill03_Hit.wav");
    SOUNDLOAD(SKILLR_CHARGING, L"hyunwoo_Skill04_Charging.wav");
    SOUNDLOAD(SKILLR_HIT, L"hyunwoo_Skill04_Hit.wav");

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
    SOUNDLOAD(NORMAL_ATTACK, L"Gun_Fire.wav");
    SOUNDLOAD(SKILLQ, L"aya_Skill01_Attack_v1.wav");
    SOUNDLOAD(SKILLW, L"aya_Skill02_Shot.wav");
    SOUNDLOAD(SKILLE, L"aya_Skill03_Activation.wav");
    SOUNDLOAD(SKILLE_CHAR, L"Aya_PlaySkill1002400seq0_3_ko.wav");
    SOUNDLOAD(SKILLR_READY, L"aya_Skill04_Ready.wav");
    SOUNDLOAD(SKILLR_CHAR, L"Aya_PlaySkill1002500seq0_1_ko.wav");
    SOUNDLOAD(SKILLR_ACTION, L"aya_Skill04_Activation.wav");

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

#pragma once

struct tInitial_Character_Stats
{
	// 기본 능력치
	short iAttackPower;					// 공격력
	short iAttackPowerPerLevel;			// 레벨당 공격력
	short iDefense;						// 방어력
	short iDefensePerLevel;				// 레벨당 방어력
	short iMaxHP;						// 체력
	short iMaxHPPerLevel;				// 레벨당 체력
	float fHPRegen;						// 체력 재생
	float fHPRegenPerLevel;				// 레벨당 체력 재생
	short iMaxSP;						// 스테미너
	short iMaxSPPerLevel;				// 레벨당 스테미너
	float fSPRegen;						// 스테미나 재생
	float fSPRegenPerLevel;				// 레벨당 스테미너 재생
	float fAttackSpeed;					// 공격 속도 %
	float fCriticalStrikeChance;		// 치명타 확률
	float fMovementSpeed;				// 이동 속도
	float fVisionRange;					// 시야

	UINT	eWPType;					// 주무기 타입
	float	fWpAtkSpd;					// 주무기 공격 속도
	float	fWpAtkRange;				// 주무기 사거리
}typedef ER_Initial_Stats;

struct tIngame_Stats
{
	int		iLevel;					// 레벨
	int		iExp;					// 경험치
	int		iNeedExpForNextLevel;	// 다음레벨 경험치

	int		iAttackPower;			// 공격력
	int		iDefense;				// 방어력
	int		iMaxHP;					// 최대 체력
	int		iHP;					// 현재 체력
	float	fHPRegen;				// 체력 재생
	int		iMaxSP;					// 최대 스태미너
	int		iSP;					// 현재 스태미너
	float	fSPRegen;				// 스태미너 재생
	float	fAttackSpeed;			// 공격속도
	float	fCriticalStrikeChance;	// 치명타 확률
	float	fMovementSpeed;			// 이동속도
	float	fVisionRange;			// 시야
	float	fAtakRange;				// 기본공격 사거리

	float	fCriticalStrikeDamage;
	float	fCooldownReduction;			// 쿨타임 감소
	float	fOmnisyphon;					// 모든 피해 흡혈
	int		iSkillAmplification;	// 스킬 증폭

	void Init_To_LevelOne(const ER_Initial_Stats& _StdStats)
	{
		iLevel = 1;
		iExp = 1;
		iNeedExpForNextLevel = 50;	// 임의 설정

		iAttackPower = _StdStats.iAttackPower;
		iDefense = _StdStats.iDefense;
		iMaxHP = _StdStats.iMaxHP;
		iHP = _StdStats.iMaxHP;
		fHPRegen = _StdStats.fHPRegen;
		iMaxSP = _StdStats.iMaxSP;
		iSP = _StdStats.iMaxHP;
		fSPRegen = _StdStats.fSPRegen;
		fAttackSpeed = _StdStats.fAttackSpeed;
		fCriticalStrikeChance = _StdStats.fCriticalStrikeChance;
		fMovementSpeed = _StdStats.fMovementSpeed;
		fVisionRange = _StdStats.fVisionRange;
		fAtakRange = _StdStats.fWpAtkRange;

		fCriticalStrikeDamage = 0;
		fCooldownReduction = 0;
		fOmnisyphon = 0;
		iSkillAmplification = 0;
	};
}typedef ER_Ingame_Stats;

#include <Engine\ptr.h>
#include <Engine\CTexture.h>

struct tItem_Stats
{
	short iAttackPower;					// 공격력
	short iAttackPowerPerLevel;			// 레벨당 공격력
	short iSkillAmplification;			// 스킬 증폭
	short iSkillAmplificationPerLevel;	// 레벨당 스킬 증폭
	short iMaxHP;						// 체력
	short iMaxHPPerLevel;				// 레벨당 체력
	float fHPRegen;						// 체력 재생
	float fHPRegenPerLevel;				// 레벨당 체력 재생
	short iMaxSP;						// 스테미너
	short iMaxSPPerLevel;				// 레벨당 스테미너
	float fSPRegen;						// 스테미나 재생
	float fSPRegenPerLevel;				// 레벨당 스테미너 재생
	short iDefense;						// 방어력
	float fAttackSpeed;					// 공격 속도 %
	float fCriticalStrikeChance;		// 치명타 확률
	float fCriticalStrikeDamage;		// 치명타 추가 데미지
	float fMovementSpeed;				// 이동 속도
	float fVisionRange;					// 시야
	float fCooldownReduction;			// 쿨타임 감소
	float fOmnisyphon;					// 모든 피해 흡혈
	short padding;						// 2byte 패딩

}typedef ER_ItemStats;

struct tSkill_Info
{
	wstring			strName;	// 스킬 이름
	Ptr<CTexture>	TexSkill;	// 스킬 아이콘

	int		iMaxSkillLevel;		// 최대 스킬 레벨

	// 기본 스킬 정보
	int		iValue1[5];			// Int형 인자
	int		iValue2[5];			// Int형 인자
	float	fValue1[5];			// float형 인자
	float	fValue2[5];			// float형 인자
	
	float	fRange[5];			// 스킬 범위 인자
	float	fMaxCoolDown[5];	// 재사용 대기시간

	// 인게임 정보
	int		iSkillLevel;		// 스킬 레벨	: value Idx
	float	fCoolDown;			// 남은 재사용 대기시간

	tSkill_Info()
		: iMaxSkillLevel(5)	// 최대 스킬 레벨
		, iValue1{}
		, iValue2{}
		, fValue1{}
		, fValue2{}
		, fRange{}
		, fMaxCoolDown{}
		, iSkillLevel(1)
		, fCoolDown(0.f)
	{
		strName = L"NULL";
	}

	void Save(FILE* _File)
	{
		SaveWString(strName, _File);
		fwrite(&iMaxSkillLevel, sizeof(int), 1, _File);
		fwrite(&iValue1, sizeof(int), 5, _File);
		fwrite(&iValue2, sizeof(int), 5, _File);
		fwrite(&fValue1, sizeof(float), 5, _File);
		fwrite(&fValue2, sizeof(float), 5, _File);
		fwrite(&fRange, sizeof(float), 5, _File);
		fwrite(&fMaxCoolDown, sizeof(float), 5, _File);
	}

	void Load(FILE* _File)
	{
		LoadWString(strName, _File);
		fread(&iMaxSkillLevel, sizeof(int), 1, _File);
		fread(&iValue1, sizeof(int), 5, _File);
		fread(&iValue2, sizeof(int), 5, _File);
		fread(&fValue1, sizeof(float), 5, _File);
		fread(&fValue2, sizeof(float), 5, _File);
		fread(&fRange, sizeof(float), 5, _File);
		fread(&fMaxCoolDown, sizeof(float), 5, _File);
	}

}typedef ER_SKILL;

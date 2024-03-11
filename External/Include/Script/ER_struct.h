#pragma once

struct tStandard_Character_Stats
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
}typedef ER_STD_CHAR_STATS;

struct tIngame_Character_Stats
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

	int		iSkillAmplification;	// 스킬 증폭

}typedef ER_Ingame_Stats;

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
#pragma once
#include "ER_define.h"

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
	float	fAtkRange;				// 기본공격 사거리

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
		iSP = _StdStats.iMaxSP;
		fSPRegen = _StdStats.fSPRegen;
		fAttackSpeed = _StdStats.fAttackSpeed;
		fCriticalStrikeChance = _StdStats.fCriticalStrikeChance;
		fMovementSpeed = _StdStats.fMovementSpeed;
		fVisionRange = _StdStats.fVisionRange;
		fAtkRange = _StdStats.fWpAtkRange;

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
	int		iValue1[6];			// Int형 인자
	int		iValue2[6];			// Int형 인자
	float	fValue1[6];			// float형 인자
	float	fValue2[6];			// float형 인자
	
	float	fRange[6];			// 스킬 범위
	int		iUseSP[6];			// 사용스테미너
	float	fMaxCoolDown[6];	// 재사용 대기시간
	float	fMaxActionTime[6];	// 최대 지속시간

	// 인게임 정보
	int		iSkillLevel;		// 스킬 레벨	: value Idx
	int		iCurUseSP;			// 현재 사용에 필요한 SP
	float	fCoolDown;			// 남은 재사용 대기시간
	bool	IsUsable;			// 사용 가능 여부;
	bool	IsAction;			// 작동중인지 여부;
	float	fActionTime;		// 지속시간


	tSkill_Info()
		: iMaxSkillLevel(5)	// 최대 스킬 레벨
		, iValue1{}
		, iValue2{}
		, fValue1{}
		, fValue2{}
		, fRange{}
		, iUseSP{}
		, fMaxCoolDown{}
		, fMaxActionTime{}
		, iSkillLevel(0)
		, iCurUseSP(0)
		, fCoolDown(0.f)
		, IsUsable(true)
		, IsAction(false)
		, fActionTime(0.f)
	{
		strName = L"NULL";
	}

	virtual ~tSkill_Info() {}

public:
	bool Use(int* _CharacterSP, bool _IsBuf = false)
	{
		int UsedSP = *_CharacterSP - iUseSP[iSkillLevel];
		
		if (UsedSP < 0)
			return false;

		if (IsUsable)
		{
			if (_IsBuf)
			{
				fActionTime = fMaxActionTime[iSkillLevel];
				IsAction = true;
			}

			fCoolDown = fMaxCoolDown[iSkillLevel];
			IsUsable = false;

			*_CharacterSP = UsedSP;
			return true;
		}

		return false;
	}

	const int& Int1() { return iValue1[iSkillLevel]; }
	const int& Int2() { return iValue2[iSkillLevel]; }
	const float& Float1() { return fValue1[iSkillLevel]; } 
	const float& Float2() { return fValue2[iSkillLevel]; } 
	const float& Range() { return fRange[iSkillLevel]; } 
	const float& MaxCooldown() { return fMaxCoolDown[iSkillLevel]; }
	const float& CurCooldown() { return fCoolDown; }
	const float& ActionTime() { return fMaxActionTime[iSkillLevel]; }
	const int& UseSP() { return iUseSP[iSkillLevel]; }
	void ActionOver() { fActionTime = 0.f; }

	void SkillStatusUpdate(float _Ratio)
	{
		iCurUseSP = iUseSP[iSkillLevel];

		if (IsAction)
		{
			fActionTime -= _Ratio;
			if (fActionTime <= 0)
			{
				fActionTime = 0;
				IsAction = false;
			}
		}

		if (IsUsable)
			return;

		fCoolDown -= _Ratio;

		if (fCoolDown <= 0)
		{
			IsUsable = true;
			fCoolDown = fCoolDown = fMaxCoolDown[iSkillLevel];
		}

	}


	void Save(FILE* _File)
	{
		SaveWString(strName, _File);
		fwrite(&iMaxSkillLevel, sizeof(int), 1, _File);
		fwrite(&iValue1, sizeof(int), 6, _File);
		fwrite(&iValue2, sizeof(int), 6, _File);
		fwrite(&fValue1, sizeof(float), 6, _File);
		fwrite(&fValue2, sizeof(float), 6, _File);
		fwrite(&fRange, sizeof(float), 6, _File);
		fwrite(&iUseSP, sizeof(int), 6, _File);
		fwrite(&fMaxCoolDown, sizeof(float), 6, _File);
		fwrite(&fMaxActionTime, sizeof(float), 6, _File);
	}

	void Load(FILE* _File)
	{
		LoadWString(strName, _File);
		fread(&iMaxSkillLevel, sizeof(int), 1, _File);
		fread(&iValue1, sizeof(int), 6, _File);
		fread(&iValue2, sizeof(int), 6, _File);
		fread(&fValue1, sizeof(float), 6, _File);
		fread(&fValue2, sizeof(float), 6, _File);
		fread(&fRange, sizeof(float), 6, _File);
		fread(&iUseSP, sizeof(int), 6, _File);
		fread(&fMaxCoolDown, sizeof(float), 6, _File);
		fread(&fMaxActionTime, sizeof(float), 6, _File);
	}

}typedef ER_SKILL;

struct tStatus_Effect
{
private:
	UINT Effect;
	float effectStats[10];
	float fActionTime[10];

public:
	const UINT& GetCurStatus() { return Effect; }

	int GetIncATK() { return (int)effectStats[0]; }
	int GetIncDEF() { return (int)effectStats[1]; }
	float GetIncSPD() { return effectStats[2]; }
	float GetIncAPD() { return effectStats[3]; }
	int GetDecATK() { return (int)effectStats[4]; }
	int GetDecDEF() { return (int)effectStats[5]; }
	float GetDecSPD() { return effectStats[6]; }
	float GetDecAPD() { return effectStats[7]; }
	bool IsFear(UINT _bufndbuf) { return _bufndbuf & (UINT)eStatus_Effect::FEAR; }
	bool IsStun(UINT _bufndbuf) { return _bufndbuf & (UINT)eStatus_Effect::STUN; }

	bool ActiveEffect(UINT ActiveEffect, float _ActionTime, float value = 0.f)
	{
		for (int i = 0; i < 10; i++)
		{
			if (ActiveEffect & 1 << i)
			{
				Effect |= ActiveEffect;			// 발견한 효과 적용
				effectStats[i] = value;			// 효과 수치 적용
				fActionTime[i] = _ActionTime;	// 작용시간 적용
				return true;
			}
		}
		// 아무것도 확인되지 않았으면 없는 효과
		return false;
	}

	void ActionTiemUpdate(float timeratio)
	{
		// 적용된 효과 없음
		if (0 == Effect)
			return;

		for (int i = 0; i < 10; ++i)
		{
			if (Effect & 1 << i)
			{
				fActionTime[i] -= timeratio;
				
				// 지속시간 종료
				if (fActionTime[i] <= 0)
				{
					fActionTime[i] = 0;
					effectStats[i] = 0;
					Effect &= ~(1 << i);
				}
			}
		}
	}

	tStatus_Effect()
		: Effect(0)
		, effectStats{}
		, fActionTime{}
	{}

} typedef BUFNDEBUF_STATS;

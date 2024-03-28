#include "pch.h"
#include "ER_EffectScript.h"

ER_EffectScript::ER_EffectScript()
	: CScript((UINT)SCRIPT_TYPE::ER_EFFECTSCRIPT)
	, m_fTime(0.f)
{
}

ER_EffectScript::~ER_EffectScript()
{
}

void ER_EffectScript::begin()
{
}

void ER_EffectScript::tick()
{
}

void ER_EffectScript::SaveToLevelFile(FILE* _File)
{
}

void ER_EffectScript::LoadFromLevelFile(FILE* _FILE)
{
}
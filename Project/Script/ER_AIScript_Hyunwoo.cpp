#pragma once
#include "pch.h"
#include "ER_AIScript_Hyunwoo.h"

#include "ER_AIScript_Nodes.cpp"

ER_AIScript_Hyunwoo::ER_AIScript_Hyunwoo()
	: ER_AIScript(SCRIPT_TYPE::ER_AISCRIPT_HYUNWOO)
{
}

ER_AIScript_Hyunwoo::~ER_AIScript_Hyunwoo()
{
}

void ER_AIScript_Hyunwoo::init()
{
	RegistAllItemBox((UINT)LUMIAISLAND::SCHOOL);
	Selector_Node* Sel_BattleCommand = SetERCharacterAIBase();
	SetBattleStartTime(120.f);
}

void ER_AIScript_Hyunwoo::begin()
{
}

void ER_AIScript_Hyunwoo::tick()
{
}
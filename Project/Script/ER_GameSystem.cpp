#include "pch.h"
#include "ER_GameSystem.h"

#include "ER_BattleSystem.h"

#include "ER_ItemMgr.h"
#include "ER_CharacterMgr.h"

ER_GameSystem::ER_GameSystem()
{
}

ER_GameSystem::~ER_GameSystem()
{
}

void ER_GameSystem::init()
{
	ER_ItemMgr::GetInst()->init();
	ER_CharacterMgr::GetInst()->init();
	ER_BattleSystem::GetInst()->init();


}

void ER_GameSystem::progress()
{
	tick();
}

void ER_GameSystem::tick()
{
	// Manager tick
}
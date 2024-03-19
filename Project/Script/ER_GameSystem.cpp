#include "pch.h"
#include "ER_GameSystem.h"

#include "ER_ItemMgr.h"
#include "ER_CharacterMgr.h"
#include "ER_UIMgr.h"

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
	ER_UIMgr::GetInst()->init();
}

void ER_GameSystem::progress()
{
	tick();
}

void ER_GameSystem::tick()
{
	// Manager tick
	ER_UIMgr::GetInst()->tick();
}
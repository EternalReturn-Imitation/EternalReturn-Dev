#include "pch.h"
#include "ER_GameSystem.h"

#include "ER_ItemMgr.h"

ER_GameSystem::ER_GameSystem()
{
}

ER_GameSystem::~ER_GameSystem()
{
}

void ER_GameSystem::init()
{
	ER_ItemMgr::GetInst()->init();
}

void ER_GameSystem::progress()
{
}

void ER_GameSystem::tick()
{
}

void ER_GameSystem::render()
{
}

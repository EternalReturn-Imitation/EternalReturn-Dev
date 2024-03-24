#include "pch.h"
#include "ER_GameSystem.h"

#include "ER_BattleSystem.h"

#include "ER_UIMgr.h"
#include "ER_ItemMgr.h"
#include "ER_CharacterMgr.h"
#include "ER_ProjectilePool.h"

#include "ER_Cursor.h"

ER_GameSystem::ER_GameSystem()
	: m_pCursor(nullptr)
{
}

ER_GameSystem::~ER_GameSystem()
{
}

ER_Cursor* ER_GameSystem::GetCursor()
{
	return m_pCursor->GetScript<ER_Cursor>();
}

void ER_GameSystem::GameStart()
{
	ER_UIMgr::GetInst()->SpawnUI();
	SpawnGameObject(m_pCursor, L"UI");
}

void ER_GameSystem::init()
{
	ER_ItemMgr::GetInst()->init();
	ER_CharacterMgr::GetInst()->init();
	ER_BattleSystem::GetInst()->init();
	ER_ProjectilePool::GetInst()->init();
	ER_UIMgr::GetInst()->init();

	m_pCursor = new CGameObject;
	m_pCursor->AddComponent(new ER_Cursor);
	m_pCursor->GetScript<ER_Cursor>()->init();
}

void ER_GameSystem::progress()
{
	tick();
}

void ER_GameSystem::tick()
{
}
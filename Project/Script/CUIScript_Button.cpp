#include "pch.h"
#include "CUIScript_Button.h"

#include "ER_GameSystem.h"
#include "ER_ActionScript_Character.h"
#include "ER_DataScript_Character.h"
#include "ER_DataScript_ItemBox.h"

CUIScript_Button::CUIScript_Button()
	: CScript((UINT)SCRIPT_TYPE::UISCRIPT_BUTTON)
	, m_iUIPos(-1)
	, m_eUICase(UICASEEND)
	, m_eBtnCase(BTNCASEEND)
{
}

CUIScript_Button::CUIScript_Button(const CUIScript_Button& _origin)
	: CScript((UINT)SCRIPT_TYPE::UISCRIPT_BUTTON)
{
}

CUIScript_Button::~CUIScript_Button()
{
}

void CUIScript_Button::init()
{
}

void CUIScript_Button::begin()
{
	// GetOwner()->UI_Button()->SetUIScript(static_cast<CScript*>(this));
	// GetOwner()->UI_Button()->SetUIFunc_CsrClick(static_cast<CUI_DELEGATE>(&CUIScript_Button::ItemAcquisiton));
}

void CUIScript_Button::tick()
{
}

void CUIScript_Button::ItemAcquisiton()
{
	if (m_eBtnCase == BOXBUTTON) {
		CGameObject* player = ER_GameSystem::GetInst()->GetPlayerCharacter();
		ER_ActionScript_Character* AC = player->GetScript<ER_ActionScript_Character>();
		ER_DataScript_Character* DC = player->GetScript<ER_DataScript_Character>();
		
		if (!(AC->GetFarmingObject()))
			return;
		
		ER_DataScript_ItemBox* DI = AC->GetFarmingObject()->GetScript<ER_DataScript_ItemBox>();
		
		CGameObject* itemBoxObj = DI->GetItem(m_iUIPos);

		if (!itemBoxObj)
			return;

		CGameObject* obj = DC->ItemAcquisition(itemBoxObj);
		if (obj == nullptr)
			return;
		else {
			DI->SetEmptyItem(m_iUIPos);
			// std::pair<CGameObject*, CGameObject*> objPair = ER_UIMgr::GetInst()->GetItemBoxItem(m_iUIPos / 4, m_iUIPos % 4);
			// objPair.first->SetEnable(false);
			// objPair.second->SetEnable(false);
		}
	}
}
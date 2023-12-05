#include "pch.h"
#include "CGameObjectEx.h"

#include <Engine\CComponent.h>

void CGameObjectEx::finaltick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::SCRIPT; ++i)
	{
		if (nullptr != GetComponent((COMPONENT_TYPE)i))
			GetComponent((COMPONENT_TYPE)i)->finaltick();
	}

	const vector<CGameObject*>& vecChild = GetChild();
	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		vecChild[i]->finaltick();
	}
}

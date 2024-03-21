#pragma once
#include <Engine\CSingleton.h>

#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

class ER_UIMgr
	: public CSingleton<ER_UIMgr>
{
	SINGLE(ER_UIMgr);
private:
	std::pair<CGameObject*, CGameObject*> m_aInventoryList[2][5];
	std::pair<CGameObject*, CGameObject*> m_aEquipList[2][3];
	CGameObject* m_pPCFace;
	CGameObject* m_pHPBar;
	CGameObject* m_pHPReturnBar;
	CGameObject* m_pStemnarBar;
	CGameObject* m_aSkillList[4];
	CGameObject* m_pItemBox;
	std::pair<CGameObject*, CGameObject*> m_aItemBoxList[2][4];
	CGameObject* m_pStatText[4][2];

	//등급 텍스트 포인터로 저장해놓기.
	Ptr<CTexture> m_pGradeTexture[5];

	//마우스 커서 비트맵
	HBITMAP m_hBitmap;
	ULONG_PTR m_uGdiplusToken;
	Gdiplus::GdiplusStartupInput m_gGdiplusStartupInput;

private:
	bool m_bHPChangeTrigger;

public:
	void init();
	void tick();

public:
	void GameStart();
	void CreateBaseUI();
	void CreateHPBar();
	void CreateSkill();
	void CreateInventorySlot();
	void CreateInventoryItem();
	void CreateEquipSlot();
	void CreateEquipItem();
	void CreateDropInventory();
	void CreateStatText();

public:
	void UpdateStat();
	void UpdateHP();
	void UpdateHPReturnBar();
	void UpdateSteminar();

	//마우스 커서 로딩
public:
	HBITMAP LoadPNGAsBitmap(LPCTSTR szFilename);
	HCURSOR BitmapToCursor(HWND hWnd, HBITMAP hBitmap);

public:
	CGameObject* GetItemBoxBackground() { return m_pItemBox; }
	std::pair<CGameObject*, CGameObject*> GetItemBoxList(int _y, int _x) { return m_aItemBoxList[_y][_x]; }
	Ptr<CTexture> GetGradeTexture(int _i) { return m_pGradeTexture[_i]; }

	std::pair<CGameObject*, CGameObject*> GetItemBoxItem(int _y, int _x) { return m_aItemBoxList[_y][_x]; }
	std::pair<CGameObject*, CGameObject*> GetInventoryItem(int _y, int _x) { return m_aInventoryList[_y][_x]; }

public:
	Vec3 WorldPosToUIPos(Vec3 worldPos);
};


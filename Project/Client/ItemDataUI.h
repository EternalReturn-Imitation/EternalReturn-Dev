#pragma once
#include "UI.h"

#include <Script\ER_DataScript_Item.h>
#include <Script\ER_define.h>
#include <Script\ER_struct.h>

enum class ItemDataUItab
{
    ITEMDATA,
    RECIPE,
    END,
};

class ItemDataUI :
    public UI
{
private:
    bool m_bOpenTab[(UINT)ItemDataUItab::END];
    string m_bTabName[(UINT)ItemDataUItab::END];
    
    vector<CGameObject*>* m_vecItem;						// 아이템 리스트

    ImGuiTableFlags     ItemDataUIFlags;

    CGameObject*        m_pCurItem;

    bool                m_bItemStatsEditWin;
    CGameObject*        m_pCurStatsEditItem;
    Ptr<CTexture>       m_pEmptyItemSlotTex;

    bool                m_bItemPopup;
    int                 m_iDeleteItemIdx;

    bool                m_bRecipePopup;

    CGameObject*        m_DragItem;
    CGameObject*        m_DropItem;
    int                 m_iDragItemIdx;
    int                 m_iDropItemIdx;

    vector<string>      m_vecItemName;

public:
    virtual void init() override;
    virtual void tick() override;
    virtual void finaltick() override;
    virtual int render_update() override;
    
    vector<string>      GetItemNameList() { return m_vecItemName; }

    void RegistItemMgr();

private:
    void render_menubar();
    void render_ItemInfoTable();
    void render_ItemStatEdit();

    void Print_Stats(const ER_ItemStats& _stats);

    void SwapItem();
    void ItemPopUp();
    void render_RecipeSearch();
    void SelectItemIcon(DWORD_PTR _data, DWORD_PTR _target);

public:
    ItemDataUI();
    virtual ~ItemDataUI();
};

